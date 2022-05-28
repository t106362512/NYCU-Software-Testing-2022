# Homework 4: Mutation Based Testing

- Tutorial

Download and install the Java mutation tool: [muJava](https://cs.gmu.edu/~offutt/mujava/). Use muJava to test  [`cal()`](https://cs.gmu.edu/~offutt/softwaretest/java/Cal.java). Use all the operators. Design tests to kill all non-equivalent mutants. Note that a test case is a method call to `cal()`.

## Tutorial

- [Slide - NYCU_Software_Testing_2022_HW_4](https://docs.google.com/presentation/d/1YVx8q_tFVj6a-Nx5t2Z3V-8WuBzjXB9fBGPP2nof7Lo)

## Questions

- How many mutants are there?

- How many test cases do you need to kill the non-equivalent mutants?

- What mutation score were you able to achieve before analyzing for equivalent mutants?

- How many equivalent mutants are there?

## How to use?

1. 安裝 jdk 1.8 (1.8 以後的版本根本開不起來, 最後裝 `graalvm-21.0.0.2+java8` 才成功)
2. 安裝 maven 後執行以下指令, 以自動化地處理前置作業

    ```sh
    mvn clean verify
    ```

3. 使用以下指令以指定 `CLASSPATH` 以及建立變異測試之檔案和執行變異測試

    ```sh
    cd tagret
    export CLASSPATH=${CLASSPATH}:${PWD}/lib/*:${JAVA_HOME}/lib/*

    # 產生變異測試之檔案, 需勾選 Mutants Generator 之所有選項, 成功後可於 `target/result` 與 Traditional Mutants Viewer 確認.
    java mujava.gui.GenMutantsMain

    # 執行變異測試, 需於 TestCase Runner 中選擇 Excute all mutants, 然後 Class, Method 和 TestCase 要重選(程式的問題), 最後按 Run 等結果.
    java mujava.gui.RunTestMain
    ```

## Report

- [310551017_HW4.md](310551017_HW4.md)
