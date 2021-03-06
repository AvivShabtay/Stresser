# Stresser

<p align="left">
    <img src="https://img.shields.io/badge/language-Node.js-yellow.svg">
    <img src="https://img.shields.io/badge/language-React.js-yellow.svg">
    <img src="https://img.shields.io/badge/language-C++-yellow.svg">
    <img src="https://img.shields.io/badge/license-MIT-blue.svg">
</p>

## Team

[Aviv Shabtay](https://github.com/AvivShabtay) |
[Elior Shriki](https://github.com/EliorShriki) |
[Or Pinhas](https://github.com/orpinhas20)

---

## Background

Stresser is a complete Endpoint Security Solution contains Central management and Agents using unique method to protect against sophisticated Malwares.

---

## Main features

- Anti-Malware protections - exploiting malware's Anti-Technics (Anti-Detection, Anti-VM, Anti-Sandbox, etc) in order to supply protection.
- Centralized Management - provides greater visibility, manage operations, unifies security with a simple UI.
- Dynamic & Static operation - support both capabilities, giving the user control of the overload on the system.
- Simple to deploy and maintains - ready-to-use suite for Windows environment.

---

## Main components

- Server - interface for clients (Web management, Agents), interface to data, manage tasks and operation of the agent.
- Web management - UI for the users, provide easy interface to configure policy, manage rules, check events and control the entire solution.
- Agent - background service, contains user-mode client and kernel-mode engine, enforce the policy and provides protection on the endpoint.

---

## Useful links

- Microsoft example for demonstrating the difference between various types of IO communication: https://github.com/microsoft/Windows-driver-samples/tree/master/general/ioctl/wdm
- OSR example of using kernel notifications (Object notification and more) can be found here: https://github.com/OSRDrivers/kmexts
- Microsoft example for demonstrating usage of Object callback: https://github.com/microsoft/Windows-driver-samples/tree/master/general/obcallback
- Microsoft example for shared event between UM application and KM driver: https://github.com/microsoft/Windows-driver-samples/tree/master/general/event

---

## Common issues
### "Inf2Cat, signability test failed." Error: 22.9.7 "DriverVer set to a date in the future"
- Possible solution is to set the `Run Inf2Cat` option (under project Properties -> Inf2Cat) to turn `Off`.
- Another solution is to set the `Use Local Time` option (under project Properties -> Inf2Cat) to `Yes`
- More information about this issues can be found here: https://stackoverflow.com/a/17005092

### `SYSTEM_THREAD_EXCEPTION_NOT_HANDLED` when calling `ObRegisterCallbacks`
- Caused by the lake of `/integritycheck` flags in driver project.
- Under project Properties -> Linker -> Command Line -> add the following flag: `/integritycheck`.
- More information about this issues can be found here: https://community.osr.com/discussion/189136/about-obregistercallbacks

### error C2371: `'PEPROCESS'` : redefinition; different basic types
- This caused by conflicts between two included headers: `ntddk.h` and `ntifs.h`.
- The solution is to only include `ntifs.h`.
- More information about this issue can be found here: https://community.osr.com/discussion/170511

### Convert resource path to relative path
- Use pre-processor symbols and create kind of "compile time-constant"
- Under your property settings to go: Resource -> General -> Preprocessor Definitions -> Edit
- Add your compile time symbol and use VS macros to get relative path, for example: `SOLUTION=$(SolutionDir)`
- Right click on your `.rc` file and choose `view code`
- Use the symbol instead of using relative paths
- For more info check my `props` folder -> `.props` file and `.rc` file

---