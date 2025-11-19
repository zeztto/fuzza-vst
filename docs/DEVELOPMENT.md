# 개발 가이드

이 문서는 Fuzza VST 플러그인의 개발 및 기여 방법을 설명합니다.

## 프로젝트 구조
- **CMakeLists.txt**: 프로젝트의 빌드 설정 파일입니다. JUCE 라이브러리를 가져오고 플러그인 대상을 정의합니다.
- **Source/**: 소스 코드가 위치한 폴더입니다.
    - `PluginProcessor.h/cpp`: 오디오 처리 로직(DSP)과 파라미터 관리를 담당합니다.
    - `PluginEditor.h/cpp`: 사용자 인터페이스(GUI)를 담당합니다.

## 오디오 처리 알고리즘

### 신호 흐름 (Signal Flow)
```
Input → Input Gate → Gain → Hard Clipping → Tone Filter → Makeup Gain → Mix → Output Suppressor → Output

```

### DSP 구현 세부사항

#### 1. Bypass
- Boolean 파라미터 (true/false)
- True일 때 모든 처리를 건너뛰고 원본 신호 출력 (트루 바이패스)

#### 2. Dual-Stage Noise Reduction
단일 Gate 노브로 두 단계의 노이즈 제어를 수행합니다.

**Stage 1: Input Gate (Pre-Distortion)**
- **목적**: 기타 험 노이즈 등 소스 노이즈 제거
- **위치**: 신호 처리 체인 최상단
- **특성**: Fast Attack (5ms) / Fast Release (50ms)
- **효과**: 연주 시작과 끝을 깔끔하게 처리

**Stage 2: Output Suppressor (Post-Mix)**
- **목적**: 하이게인 회로 노이즈 억제 및 서스테인 보존
- **위치**: Mix 이후 최종 출력단
- **특성**: Medium Attack (20ms) / Long Release (500ms)
- **Threshold**: 입력단보다 50% 낮게 설정
- **효과**: 자연스러운 페이드아웃과 긴 서스테인 확보

#### 2. Gain & Auto-Level Compensation
```cpp
float inputGain = juce::jmax(1.0f, gainParam);  // 최소 1배
float makeupGain = 2.0f / std::sqrt(inputGain); // Sqrt 기반 자동 보상
```
- **inputGain**: 입력 신호 증폭 (1배 ~ 100배)
- **makeupGain**: 자동 레벨 보상 (출력 레벨 일정하게 유지)
- **sqrt 사용 이유**: 선형보다 지각적으로 자연스러운 보상

#### 3. Hard Clipping
```cpp
sample = juce::jlimit(-1.0f, 1.0f, sample);
```
- ±1.0 범위로 신호 제한
- 클래식한 퍼즈 디스토션 효과

#### 4. Tone Filter
- **타입**: Low-pass filter (JUCE `StateVariableTPTFilter`)
- **범위**: 500Hz - 5000Hz (실제 퍼즈 페달 대역)
- **채널**: 스테레오 독립 필터 (Left/Right)

## 사용된 JUCE 모듈
- `juce_audio_utils`: 오디오 플러그인 기본 유틸리티
- `juce_dsp`: DSP 필터 (StateVariableTPTFilter)

## CI/CD (GitHub Actions)
이 프로젝트는 GitHub Actions를 사용하여 변경 사항이 `main` 브랜치에 푸시될 때마다 자동으로 빌드를 수행합니다.

### 워크플로우 세부사항
- **워크플로우 파일**: `.github/workflows/build.yml`
- **빌드 환경**:
  - macOS (latest) - VST3, AU 빌드
  - Windows (latest) - VST3, Standalone 빌드
- **결과물**:
  - macOS: `Fuzza-macOS-VST3.zip`, `Fuzza-macOS-AU.zip`
  - Windows: `Fuzza-Windows-VST3.zip`, `Fuzza-Windows-Standalone.zip`

### Artifacts 다운로드
1. GitHub Actions 탭으로 이동
2. 최신 성공한 워크플로우 실행 선택
3. Artifacts 섹션에서 원하는 빌드 다운로드

## 로컬 테스트 방법
1. 코드를 수정한 후 `cmake --build build` 명령어로 다시 빌드합니다.
2. DAW(Logic Pro, Reaper, Ableton Live 등)를 재시작하여 플러그인을 다시 스캔합니다.
3. 변경 사항이 적용되었는지 확인합니다.

### 디버깅
- Xcode로 디버깅: `cmake -B build -G Xcode` 후 Xcode에서 프로젝트 열기
- Visual Studio로 디버깅: `cmake -B build -G "Visual Studio 17 2022"` 후 솔루션 열기
