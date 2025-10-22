# PokerPro
![PokerPro Logo](https://i.ibb.co/RNJfSbM/logo-pocker-pro.png)
**PokerPro** is a card game project developed as part of the **Software Project (PSW)** course in the **Master’s program in Electrical and Computer Engineering** at [FEUP](https://sigarra.up.pt/feup/en/ucurr_geral.ficha_uc_view?pv_ocorrencia_id=558417). The project was completed on **December 12, 2023**.

This course focuses on teaching software engineering concepts and methods for designing and developing **GUI-based, client/server, or database-driven applications**, while providing practical experience in **C++ programming**, including debugging, testing, and documentation throughout the software lifecycle.

## 🧠 Technologies & Components Used

- **Qt Framework (C++/Qt 5/6)** – main development framework for GUI, networking, and unit testing.

- **Qt Test (QTest)** – used for automated unit testing of networking, game logic, and table management.

- **Qt Network Module (QTcpSocket / QHostAddress / QNetworkInterface)** – implemented TCP/IP client-server communication between players and host.

- **Qt Widgets (QMainWindow, QStackedWidget, QMessageBox)** – GUI implementation for the main menu, options, connection setup, and in-game window navigation.

- **Signals & Slots Mechanism** – asynchronous communication between network controllers, server, and UI components.

- **Custom Classes:**

  -` Network_Controller`– handles all TCP communication, message parsing, and betting actions (Bet, Call, Fold).

  - `Server_Controller` – manages server-side message broadcasting and client synchronization.

  - `table_t` – maintains game state, player lists, and card distribution.

  - `Client_t / Players_t` – represent player entities with betting and chip management logic.

  - `MachState, Bet, and Whoplay` – implement finite state machines controlling game flow (ante, flop, turn, river, winner).

- **QPixmap & QPalette** – for custom background and window design.

- **QHostAddress & QNetworkInterface** – used to detect and display host IP addresses.

- **Randomization (QRandomGenerator)** – simulates card dealing and bet values during tests.

- **Unit Tests (QCOMPARE, QTest::qWait)** – validate gameplay rules, connection integrity, and message exchange between client and server.

## User Manual

A visual guide for using the app is available here: [PokerPro User Manual](https://drive.google.com/file/d/1Er52Mq4L_OR1v7ilBQwIq48e_-T29npe/view?usp=sharing) or in the `pdf` folder in this repo. This manual provides step-by-step instructions for playing the game.
