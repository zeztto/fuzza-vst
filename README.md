# Fuzza VST

JUCE와 CMake로 제작된 일렉트릭 기타용 Fuzz 디스토션 플러그인입니다.

## 주요 기능

### 컨트롤
- **Gain (0-100)**: Fuzz 디스토션의 양을 조절합니다
  - **0**: 클린 부스트 모드 (+6dB, 디스토션 없음)
  - **1-50**: 빈티지 퍼즈 사운드 (중간 디스토션)
  - **50-100**: 모던/극한 퍼즈 사운드 (강한 디스토션)
  - **자동 레벨 보상**: Gain이 높아져도 출력 레벨이 일정하게 유지됩니다

- **Tone (500Hz - 5000Hz)**: 실제 퍼즈 페달의 톤 범위로 소리의 밝기를 조절합니다
  - **500Hz**: 어둡고 따뜻한 톤 (고주파 제거)
  - **2500Hz**: 균형잡힌 톤 (기본값)
  - **5000Hz**: 밝고 날카로운 톤 (고주파 통과)

- **Bypass**: 이펙트를 켜고 끌 수 있습니다 (트루 바이패스)

### 기술적 특징
- **Hard Clipping Distortion**: 클래식한 퍼즈 알고리즘
- **Low-Pass Tone Filter**: JUCE DSP 기반 고품질 필터
- **Auto-Level Compensation**: 지각적으로 자연스러운 sqrt 기반 자동 레벨 조정
- **Zero Latency**: 실시간 처리, 지연 없음

## 빌드 방법

### 필수 요구 사항
- CMake 3.15 이상
- C++ 컴파일러 (Clang, GCC, 또는 MSVC)
- [JUCE](https://github.com/juce-framework/JUCE) (CMake를 통해 자동으로 다운로드됩니다)

### 빌드 단계

1. **설정 (Configure)**:
   ```bash
   cmake -B build
   ```

2. **빌드 (Build)**:
   ```bash
   cmake --build build
   ```

빌드된 플러그인은 자동으로 시스템 폴더에 설치됩니다.

## 설치 방법

빌드 후 플러그인이 자동으로 설치되는 경로:
- **macOS VST3**: `~/Library/Audio/Plug-Ins/VST3/Fuzza.vst3`
- **macOS AU**: `~/Library/Audio/Plug-Ins/Components/Fuzza.component`
- **Windows VST3**: `C:\Program Files\Common Files\VST3\Fuzza.vst3`
- **Standalone**: `build/Fuzza_artefacts/Standalone/`

## 지원 플랫폼
- macOS (VST3, AU, Standalone)
- Windows (VST3, Standalone)

## 버전 히스토리
자세한 변경 사항은 [CHANGELOG.md](CHANGELOG.md)를 참조하세요.

## 라이선스
MIT
