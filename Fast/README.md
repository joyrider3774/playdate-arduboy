# Fast — Playdate Port

A port of the Arduboy game *Fast* (by filmote / Pharap) to the Playdate console.

---

## Prerequisites

Before building, make sure you have:

1. **Playdate SDK** installed
   - macOS: install via the [Playdate website](https://play.date/dev/). The SDK path is auto-detected from `~/.Playdate/config`.
   - Linux/Windows: set the `PLAYDATE_SDK_PATH` environment variable pointing to your SDK folder (e.g. `export PLAYDATE_SDK_PATH=/path/to/PlaydateSDK`).

2. **CMake 3.19+**
   ```bash
   cmake --version
   ```

3. **`playdate-cpp` submodule** initialized. From the repository root:
   ```bash
   git submodule update --init
   ```

---

## Building

All commands are run from the **repository root** (not the `Fast/` folder).

### 1. Create the build directory

```bash
mkdir build
cd build
```

### 2. Configure with CMake

**macOS** (SDK path is auto-detected):
```bash
cmake ..
```

**Linux / Windows** (set SDK path explicitly if not set already):
```bash
PLAYDATE_SDK_PATH=/path/to/PlaydateSDK cmake ..
```

### 3. Build only the Fast game

```bash
cmake --build . --target Fast
```

Or build all games in the repository:
```bash
cmake --build . --config Release
```

### 4. Locate the output

After a successful build, the `.pdx` bundle is placed inside the game folder:

```
Fast/Fast.pdx/
```

---

## Running on the Playdate Simulator

Open the Playdate Simulator, then drag and drop `Fast/Fast.pdx` onto it — or use **File → Open** and select the `Fast.pdx` folder.

---

## Running on a physical Playdate

1. Connect your Playdate via USB.
2. In the Playdate Simulator, go to **Device → Upload Game to Device** and select `Fast/Fast.pdx`.

Alternatively, use the Playdate web interface at `device.play.date` to sideload the `.pdx` directly.

---

## Troubleshooting

| Problem | Solution |
|---|---|
| `cmake ..` fails with "SDK not found" | Set `PLAYDATE_SDK_PATH` explicitly (see step 2) |
| `git submodule` errors | Run `git submodule update --init` from the repo root |
| Build errors in `playdate-cpp` | Make sure the submodule was fully initialized; re-run `git submodule update --init --recursive` |
| `.pdx` not appearing | Check that the build completed without errors; the output goes to `Fast/Fast.pdx/` |
