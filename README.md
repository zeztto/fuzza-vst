# Fuzza VST

JUCE와 CMake로 제작된 일렉트릭 기타용 Fuzz 디스토션 플러그인입니다.

## 주요 기능
- **Gain**: Fuzz 디스토션의 양을 조절합니다.
- **Tone**: 소리의 밝기를 조절합니다.
- **Volume**: 출력 레벨을 조절합니다.

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

빌드된 플러그인(VST3)은 `build` 디렉토리 또는 시스템의 기본 VST3 폴더에 생성됩니다.

## 설치 방법
- **macOS**: `Fuzza.vst3` 파일을 `~/Library/Audio/Plug-Ins/VST3/` 경로로 복사하세요.
- **Windows**: `Fuzza.vst3` 파일을 `C:\Program Files\Common Files\VST3\` 경로로 복사하세요.

## 라이선스
MIT
