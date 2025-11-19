# Changelog

All notable changes to Fuzza VST will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.3.1] - 2025-11-19

### Changed
- **Tone Knob → 3 Preset Buttons**: Replaced continuous tone knob with 3 preset buttons
  - **WARM** (800Hz): Dark, warm tone for thick fuzz
  - **BALANCED** (2000Hz): Versatile, balanced tone (default)
  - **BRIGHT** (4500Hz): Cutting, bright tone for solos
- **Clipping Dropdown → 3 Toggle Buttons**: Replaced dropdown with dedicated buttons
  - **HARD**: Classic hard clipping
  - **SOFT**: Smooth tanh saturation
  - **ASYM**: Asymmetric vintage clipping
- **Knob Count**: 4 knobs → 3 knobs (Gain, Mix, Gate)
- **UI Layout Improved**:
  - Tone preset buttons (top row)
  - Clipping mode buttons (second row)
  - 3 knobs (third row)
  - Bypass footswitch (bottom center)
- **Window Height**: 400px → 420px for better spacing

### Fixed
- Clipping dropdown no longer overlaps knobs
- Cleaner, more organized pedal interface
- Better visual hierarchy

### Technical
- Tone presets implemented as Choice parameter with 3 options
- Button-based UI instead of combo boxes for better UX
- Preset mapping in processBlock: 0=800Hz, 1=2000Hz, 2=4500Hz

## [0.3.0] - 2025-11-19

### Added
- **Mix (Dry/Wet) Control**: Blend clean and fuzzed signals for parallel processing (0-100%)
- **Gate (Noise Gate)**: Threshold-based noise gate to eliminate unwanted noise and feedback (0-100%)
- **Clipping Mode Selector**: Choose between 3 distortion algorithms
  - **Hard**: Classic hard clipping (±1.0) - aggressive fuzz
  - **Soft**: Smooth tanh clipping - warm, tube-like saturation
  - **Asymmetric**: Vintage-style asymmetric clipping - unique character
- **Professional Pedal-Style GUI**:
  - Dark grey metallic enclosure with gradient
  - 4 orange-accented rotary knobs (Gain, Tone, Mix, Gate)
  - Realistic corner screws
  - Bottom-centered bypass footswitch (red when active)
  - Clean, modern aesthetic

### Changed
- **GUI Complete Redesign**: From basic interface to realistic guitar pedal look
- **Window Size**: 400x300 → 500x400 pixels (more spacious layout)
- **Bypass Button**: Moved from top-right toggle to bottom-center footswitch (pedal authentic)
- **Color Scheme**: Black background → Professional grey gradient with orange accents
- **Knob Layout**: 2-knob horizontal → 4-knob horizontal array

### Improved
- Enhanced versatility with mix control for subtle to extreme effects
- Professional appearance matching real hardware pedals
- Better noise performance with integrated gate
- Three distinct tonal characters via clipping modes

### Technical
- Dry signal preservation for parallel processing
- Gate threshold mapping: 0-100% UI → 0-0.1 internal threshold
- Clipping mode implemented as ComboBox with 3 choices
- Lambda function for knob setup (cleaner code)

## [0.2.0] - 2025-11-19

### Added
- **Bypass 기능**: 이펙트를 켜고 끌 수 있는 토글 버튼 추가 (트루 바이패스)
- **Tone 필터 DSP 구현**: 500Hz-5kHz low-pass 필터 (실제 퍼즈 페달 대역폭)
- **자동 레벨 보상 알고리즘**: Gain이 높아져도 출력 레벨이 일정하게 유지
  - Sqrt 기반 makeup gain 알고리즘
  - Gain 0에서 클린 부스트 모드 (+6dB)
- **JUCE DSP 모듈 추가**: StateVariableTPTFilter 사용

### Changed
- **Tone 파라미터 범위 조정**: 100Hz-20kHz → 500Hz-5kHz (실제 퍼즈 페달 사양)
- **Volume 노브 제거**: Gain과 통합하여 자동 레벨 보상으로 대체
- **GUI 단순화**: 3개 노브 → 2개 노브 (Gain, Tone)
- **Gain 기본값 변경**: 50.0 유지 (중간 퍼즈)
- **Tone 기본값 변경**: 5000Hz → 2500Hz (더 균형잡힌 톤)

### Improved
- 더 직관적이고 단순한 사용자 인터페이스
- 일관된 출력 레벨로 믹싱이 더 쉬워짐
- 실제 퍼즈 페달에 가까운 톤 특성

### Technical
- DSP 알고리즘 최적화: Hard clipping → Tone filter → Makeup gain
- CMakeLists.txt에 juce_dsp 모듈 링크 추가
- 코드 리팩토링: 더 명확한 주석과 구조

## [0.0.1] - 2025-11-19

### Added
- 초기 릴리즈
- 기본 Fuzz 디스토션 이펙트 (Hard clipping)
- Gain, Tone, Volume 파라미터
- VST3, AU, Standalone 지원
- macOS 및 Windows 지원
- CI/CD 파이프라인 (GitHub Actions)
- 한글 문서 (README.md, DEVELOPMENT.md)

### Features
- Hard clipping 기반 퍼즈 알고리즘
- 3개 로터리 노브 GUI
- Zero latency 실시간 처리
- 크로스 플랫폼 빌드 시스템 (CMake)

---

## 버전 관리 규칙

- **MAJOR** (X.0.0): 호환성이 깨지는 큰 변경
- **MINOR** (0.X.0): 새로운 기능 추가 (하위 호환)
- **PATCH** (0.0.X): 버그 수정 및 작은 개선
