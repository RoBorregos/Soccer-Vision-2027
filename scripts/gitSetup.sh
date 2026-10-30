#!/usr/bin/env bash
set -euo pipefail

# Must be inside a git repository
if ! git rev-parse --is-inside-work-tree &> /dev/null; then
    echo "This script must be run from within a git repository."
    exit 1
fi
cd "$(git rev-parse --show-toplevel)"

# Check platform
case "$OSTYPE" in
    linux-gnu*) OS="Linux" ;;
    darwin*)    OS="macOS" ;;
    cygwin|msys|win32) OS="Windows" ;;
    *) echo "Unknown OS: $OSTYPE"; exit 1 ;;
esac
echo "Detected OS: $OS"

install_pkg() {  # $1 = display name, $2 = apt/brew pkg, $3 = winget id
    echo "Installing $1..."
    case "$OS" in
        Linux)   sudo apt-get install -y "$2" ;;
        macOS)   brew install "$2" ;;
        Windows) winget install -e --id "$3" ;;
    esac
}

ensure_installed() {  # $1 = command, $2 = name, $3 = pkg, $4 = winget id
    if ! command -v "$1" &> /dev/null; then
        read -r -p "$2 could not be found, do you wish to install it? (y/n) " response
        if [[ "$response" == "y" ]]; then
            install_pkg "$2" "$3" "$4"
            if ! command -v "$1" &> /dev/null; then
                echo "$2 installed, but not on PATH yet. Restart your terminal and re-run this script."
                exit 1
            fi
        else
            echo "Please install $2 manually."
            exit 1
        fi
    fi
}

ensure_installed git     "git"     git     Git.Git
ensure_installed git-lfs "git-lfs" git-lfs GitHub.GitLFS

# Check whether git-lfs is initialized
if ! git config --get filter.lfs.clean &> /dev/null; then
    read -r -p "git-lfs is not initialized, do you wish to initialize it? (y/n) " response
    if [[ "$response" == "y" ]]; then
        git lfs install
    else
        echo "Please initialize git-lfs manually."
        exit 1
    fi
fi

# Activate githooks for pre-commit and pre-push
if [ -d ".githooks" ]; then
    echo "Activating githooks for pre-commit and pre-push..."
    git config core.hooksPath .githooks
    chmod +x .githooks/* 2>/dev/null || true
else
    echo "No .githooks directory found."
    exit 1
fi