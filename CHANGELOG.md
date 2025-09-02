# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Changed

- Don't increment comm event counter for diagnostic requests (Function code 0x08)
- Check CRC before slave address in ADU handling to monitor the overall health of the bus, not just this device

## [1.0.0] - 2025-08-06

### Added

- Initial open source release
