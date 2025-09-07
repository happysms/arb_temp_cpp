# C++ Multi-Exchange Arbitrage — Template

여러 거래소를 동시에 붙여서 **스프레드 기반 아비트라지**를 돌리기 위한 C++ 템플릿
전략 엔진 / 거래소 모듈 / REST API / WebSocket 매니저를 모듈형으로 나눔.

## 구성
- **전략 엔진**: 목표가 계산, 틱 보정(ceil/floor), 대기 주문 갱신/취소, 보유 한도, 헷지
- **거래소 모듈 4종**: `binance_f`, `binance_s`, `gateio_f`, `gateio_s` (같은 인터페이스)
- **API/WS 스켈레톤**: 실 REST/WS 붙일 수 있게 최소 인터페이스 제공
- **환경변수 기반 설정**: 심볼/스프레드/수량/거래소 선택을 런타임에 변경
- **의존성 최소**: C++17 + CMake만 있으면 빌드됨

## 구조
```
.
├── CMakeLists.txt
├── main.cpp
├── util/
│   └── util.h                     # ceil_by_tick, floor_by_tick, now_ms
├── exchange/
│   └── exchange.h                 # 공통 인터페이스(추상)
├── strategy/
│   ├── strategy.h
│   └── strategy.cpp               # 주문/취소/헷지/체결 로그
├── binance_f_module/
│   ├── exchange.h / exchange.cpp
│   ├── api.h / api.cpp
│   └── websocket_manager.h / websocket_manager.cpp
├── binance_s_module/              # 동일 구조
├── gateio_f_module/               # 동일 구조
└── gateio_s_module/               # 동일 구조
```

## 빠르게 실행
필요: CMake ≥ 3.14, C++17 컴파일러

```bash
mkdir build && cd build
cmake ..
make -j
./project_name
```

## 환경변수 (예시)
```bash
export TICKER=BTC_USDT
export MAX_QTY=1.0        # 최대 순보유 한도
export TRADE_QTY=0.02     # 1회 주문 수량
export SPREAD=0.001       # 0.1%
# export CANCEL_BUFFER=0.0005   # (옵션) 목표가 주변 허용 범위
export MAKER_EXCHANGE=binance_f
export TAKER_EXCHANGE=gateio_f
./question4
```

### 지원 거래소 키워드
- `binance_f`, `binance_s`, `gateio_f`, `gateio_s`

## 확장 포인트
- **REST 연동**: 각 모듈 `api.cpp`에 구현
- **WS 연동**: 각 모듈 `websocket_manager.cpp`에 구독/재연결/파싱 추가
- **신규 거래소**: `exchange/exchange.h` 상속해서 새 모듈 만들기
- **틱/최소수량/심볼 설정**: 각 모듈 `exchange.cpp` 초기화부에서 관리