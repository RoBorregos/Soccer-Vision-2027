# PlatformIO compiler image for firmware/ (Teensy 4.1).
# Holds only the toolchain. Project sources are mounted at /workspace at run time.
FROM python:3.12-slim

ENV PLATFORMIO_CORE_DIR=/opt/platformio \
    PLATFORMIO_CACHE_DIR=/tmp/pio-cache

RUN pip install --no-cache-dir platformio \
 && pio pkg install --global --platform teensy --tool framework-arduinoteensy --tool tool-teensy --tool tool-scons \
 && rm -rf /root/.cache /tmp/pio-cache

WORKDIR /workspace
ENTRYPOINT ["pio", "run", "-d", "/workspace"]
