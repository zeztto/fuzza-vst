# 개발 가이드

이 문서는 Fuzza VST 플러그인의 개발 및 기여 방법을 설명합니다.

## 프로젝트 구조
- **CMakeLists.txt**: 프로젝트의 빌드 설정 파일입니다. JUCE 라이브러리를 가져오고 플러그인 대상을 정의합니다.
- **Source/**: 소스 코드가 위치한 폴더입니다.
    - `PluginProcessor.h/cpp`: 오디오 처리 로직(DSP)과 파라미터 관리를 담당합니다.
    - `PluginEditor.h/cpp`: 사용자 인터페이스(GUI)를 담당합니다.

## CI/CD (GitHub Actions)
이 프로젝트는 GitHub Actions를 사용하여 변경 사항이 `main` 브랜치에 푸시될 때마다 자동으로 빌드를 수행합니다.
- **워크플로우 파일**: `.github/workflows/build.yml`
- **빌드 환경**: macOS (latest) 및 Windows (latest)
- **결과물**: 빌드가 성공하면 Artifacts에서 VST3 파일을 다운로드할 수 있습니다.

## 로컬 테스트 방법
1. 코드를 수정한 후 `cmake --build build` 명령어로 다시 빌드합니다.
2. DAW(Logic Pro, Reaper 등)를 실행하여 플러그인을 다시 스캔합니다.
3. 변경 사항이 적용되었는지 확인합니다.
