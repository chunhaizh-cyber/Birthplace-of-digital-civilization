# Security Policy

## Supported Versions

`main` is an active development branch. Book and review references should use fixed tags such as `v0.1-book-rc2`.

Security fixes, when applicable, will target the active branch first and may be back-noted against fixed book tags if the issue affects a published snapshot.

## Reporting a Vulnerability

Please do not open a public issue with exploit details.

Preferred reporting path:

1. Use GitHub private vulnerability reporting for this repository if it is enabled.
2. If private reporting is not available, contact the repository owner through the GitHub owner profile and provide a minimal description first.

Useful report details include:

- affected commit or tag
- operating system and build configuration
- whether the issue requires local code execution, hardware access, or crafted input
- reproduction steps with the smallest non-destructive example possible
- any relevant logs with secrets, tokens, personal data, and machine-specific paths removed

## Scope

This repository is a research prototype. Security reports are most useful when they concern:

- unsafe parsing or file handling
- unsafe process, DLL, or WebView2 loading behavior
- unintended network, filesystem, or device access
- denial-of-service issues caused by malformed local input
- third-party dependency risks that affect the bundled or documented build path

The project is not a production AI service, productized robot controller, medical device, safety controller, or proof that a digital life system is complete.

## Disclosure

After a report is acknowledged, maintainers will try to confirm impact, prepare a fix or mitigation, and coordinate public disclosure notes when appropriate. Response timing is best-effort for this research repository.
