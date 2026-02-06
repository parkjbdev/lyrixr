# 📝 TTML-based Real-time Subtitle Renderer

C로 구현된 **TTML(XML) 자막 동기화 출력기**입니다. 타임스탬프(`begin`, `end`)가 포함된 XML 파일을 파싱하여, 터미널에 실시간으로 자막을 타이핑 효과와 함께 출력합니다.

## 🏗 Project Structure

```text
.
├── src/         # .c 소스 파일
├── include/     # .h 헤더 파일
├── bin/         # 빌드 결과물 (Binary & Objects)
└── Makefile     # 빌드 자동화 스크립트
```

## 🛠 Features

* **Precise Sync**: `gettimeofday`를 활용하여 선언된 타임스탬프에 맞춰 텍스트 출력.
* **Smart UTF-8 Handling**: 멀티바이트 문자를 분석하여 한글 및 이모지 출력 시 깨짐 방지.
* **Typewriter Effect**: 자막 구간 길이에 맞춰 글자 단위 딜레이(ms) 자동 계산.
* **Robust Build**: 구조화된 디렉토리 기반의 Makefile 시스템.

## 🚀 Getting Started

### Prerequisites
`libxml2` 라이브러리가 설치되어 있어야 합니다.
```bash
# macOS
brew install libxml2

# Ubuntu/Debian
sudo apt-get install libxml2-dev
```

### Build & Run
프로젝트 루트에서 `make` 명령어를 사용하세요.

```bash
# 빌드
make

# 실행 (예시)
./bin/program sample.xml

# 빌드와 동시에 실행
make run
```

## 📂 Makefile Guide

| Command | Description |
| :--- | :--- |
| `make` | 소스 컴파일 및 `bin/program` 생성 |
| `make run` | 빌드 후 즉시 실행 |
| `make clean` | 빌드된 바이너리 및 오브젝트 파일 삭제 |

---
**Note:** macOS 환경에서 헤더 참조 에러가 발생할 경우, Makefile의 `CFLAGS`에 `$(shell xml2-config --cflags)`를 추가해 주세요.
