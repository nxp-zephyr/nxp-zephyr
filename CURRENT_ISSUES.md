# Current Issues Being Discussed

This document tracks the current open issues being actively discussed in the nxp-zephyr repository. It provides a summary and quick reference for developers and contributors.

Last Updated: 2026-01-22

## Overview

There are currently 2 open issues being discussed in the repository, both related to the IMXRT6XX Xtensa core linker script functionality.

---

## Issue #15: IMXRT6XX Linker Script - Code and Data Relocation Support

**Status:** Open  
**Reported by:** peters-r  
**Created:** 2025-10-01  
**Labels:** bug  
**Impact:** Critical features missing

### Description

The IMXRT6XX linker script for the Xtensa core does not support code and data relocation features. Methods described in NXP application note AN14597 do not work for the Xtensa core:

- Defining memory sections in the device tree
- `zephyr_code_relocate` command in CMakeLists.txt

### Technical Details

The linker script at `soc/nxp/imxrt/imxrt6xx/hifi4/linker.ld` is missing imports and definitions required to support these features.

### Reproduction Steps

1. Follow the application note: https://docs.nxp.com/bundle/AN14597/page/topics/using_the_zephyr_code_relocation_feature.html
2. Define custom memory regions in the device tree:
   ```dts
   sdram_code: memory@0 {
       device_type = "memory";
       reg = <0x00000000 DT_SIZE_M(16)>;
       compatible = "zephyr,memory-region";
       zephyr,memory-region = "SDRAMCODE";
   }
   ```
3. Attempt to use `zephyr_code_relocate` in CMakeLists.txt
4. Observe that custom memory regions have no impact on the Xtensa linker map
5. Observe that `zephyr_code_relocate` has no effect

### Affected Environments

All environments using IMXRT6XX with Xtensa core are affected.

### Discussion

See [Issue #15 Discussion](https://github.com/nxp-zephyr/nxp-zephyr/issues/15) for ongoing discussion.

---

## Issue #14: IMXRT6XX Linker Script - Linker Script Overlay Support

**Status:** Open  
**Reported by:** peters-r  
**Created:** 2025-10-01  
**Labels:** bug  
**Impact:** Critical functionality not supported

### Description

Providing custom linker script overlays does not work for the IMXRT6XX Xtensa core. The linker script is missing the necessary definitions or imports to pull in user-defined overlays.

### Technical Details

The linker script at `soc/nxp/imxrt/imxrt6xx/hifi4/linker.ld` needs modifications to support custom linker script overlays.

### Reproduction Steps

1. Create a custom linker script overlay file (e.g., `my_overlay.ld`) with custom linker sections
2. Add the following to CMakeLists.txt:
   ```cmake
   zephyr_linker_sources(SECTIONS src/my_overlay.ld)
   ```
3. Build the project
4. Check `zephyr.map` in the build folder
5. Observe that the custom linker sections are missing from the map file

### Affected Environments

All environments using IMXRT6XX with Xtensa core are affected.

### Discussion

See [Issue #14 Discussion](https://github.com/nxp-zephyr/nxp-zephyr/issues/14) for ongoing discussion.

---

## Contributing

If you're interested in helping resolve these issues, please:

1. Review the issue discussions on GitHub
2. Comment on the specific issue if you have insights or questions
3. Submit a pull request with proposed fixes following the [contribution guidelines](CONTRIBUTING.rst)

## Additional Resources

- [NXP Application Note AN14597](https://docs.nxp.com/bundle/AN14597/page/topics/using_the_zephyr_code_relocation_feature.html)
- [Zephyr Project Documentation](https://docs.zephyrproject.org/)
- [GitHub Issues](https://github.com/nxp-zephyr/nxp-zephyr/issues)
