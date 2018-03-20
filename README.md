<h1 align="center">Embedded System - Mini Project</br>"Defuse The Bomb"</h1>

<p align="center">
    <b>Final project</br>
Course 10110, Embedded System</b></p>


## ***Introduction:***
This is the final project for an Embedded System Course (10110).  
In this course we've studied a specific board that was provided to us in class.  
As a final project I've decided to make a game called "Defuse The Bomb".


## ***Game Explanation:***
This game is quite simple, once we press "A" to start playing, the player has 1 minute in to solve four mathematical questions with increasing diffculity, every right answer will grant the player a code number and a number that represents it's "index" (this is called "line" number in the game).  
After solving correctly all four problems, the player has to press "#" to input the code for the bomb, if the code is right the bomb will turn off and stop ticking! else... well we all know what happens next ;)


## ***Hardware:***
- The board itself:  
![Image of board](https://image.ibb.co/kHgexx/IMG_20180319_190215.jpgs=200 "Copyrights Afeka College")  
![Image2 of board](https://image.ibb.co/cktL4c/Picture1.jpgs=200 "Copyrights Afeka College")
- Board's processor: MICROCHIP [PIC32MX460F512L](https://www.microchip.com/wwwproducts/en/PIC32MX460F512L)
- In-Circuit Emulator: [MPLAB REAL ICE](http://www.microchip.com/Developmenttools/ProductDetails.aspx?PartNO=DV244005)  
>How It's Used:  
![](https://xsi.wdfiles.com/local--files/tool:realice/PC-RealICE-Explorer16.png "MPLAB®")


## ***Modules Used:***
As part of the project requirements I had to use at least 4 modules from the board.   
I've used the following modules:  
1. LCD Display - It's purpose is to communicate with the player and show him messages.
2. Keyboard - The player's input delivery is done with this keyboard.
3. Buzzer - It ticks faster as time goes down to imitate a real bomb.
4. Timer - With the timer we can tell when one minute has passed or when the buzzer has to tick faster as time goes down.
5. Interrupts - The interrupts mechanism listens for the Timer and reacts according to the time passed, for example: as time goes down the mechanism interrups the Buzzer and reactivates it to tick faster.
6. LCD-G - Graphical LCD is used to show the player the mathematical question he has to solve.
