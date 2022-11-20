# MCTS - Tic Tac Toe

**:star2: Monte Carlo Tree Search : Tic Tac Toe application :star2:**

Graphic toy written in C++/Qt that implements a bot that can play [**Tic Tac Toe**](https://en.wikipedia.org/wiki/Tic-tac-toe).

Under the hood, it uses the [**mcts library**](https://github.com/MericLuc/mcts) that provides a framework to apply MCTS to concrete problems.

![example](imgs/example.gif)

## How to build/install

_Dependencies_ :
  - Qt5 (Qt5::Widgets Qt5::Svg packages)
  - mcts (as a submodule)

Start by getting the sources :

```
[~/git] git clone https://github.com/MericLuc/mcts-tic-tac-toe.git
[~/git] git submodule update --init --recursive
```

Then, the easiest way to build is to use it as a QtCreator project.

Otherwise, you can build from command-line using cmake and defining a bunch of Qt related variables.

It should be something like that (not garanteed) : 

```
[~/builds] cmake -S ${PATH_TO_PROJECT} -B ${PATH_TO_BUILD} -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=${PATH_TO_QT_INSTALL}
[~/builds] make
```

## Example usage

[**Play**](https://mericluc.github.io/mcts/tic-tac-toe/app.html) in your browser using a [webassembly](https://webassembly.org/) compiled version.

## Acknowledgements

This application is inspired by [This project](https://github.com/Konijnendijk/cpp-mcts) that does pretty much the same, but better. 😁

If you are interrested by this application, you should definitely check it out.