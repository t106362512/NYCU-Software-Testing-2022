# Lab 8: Symbolic Execution

1. 我們提供一個 linux binary
   - 請透過 angr 產生出他的 cfg
   - 請找到能夠讓該程式印出 correct\n 的輸入
2. 繳交方式 : 學號.zip (zip內包含以下檔案)
   - target.cfg : 用來找位置的 cfg 圖片
   - solve.py : 執行 angr 的 python (please use python3)
   - flag.txt : 正確的參數輸入 (argv[1])

## 使用方式

1. 安裝 [poetry](https://python-poetry.org/) 與 [graphviz](https://graphviz.org/download/)
2. 使用指令以下指令安裝套件及建立環境

   ```sh

   poetry install
   ```

3. 使用指令以取得答案

   ```sh
   poetry run python cfg.py
   poetry run python solve.py
   ```
