User Manual

Introduction & Rule
Hello and welcome to the game of “See who’s fastest”!
This is a easy poker game played among three players. Firstly, only one deck of cards will be used, 54 cards including two jokers. Then, each player will be assigned 18 random cards, and the system will randomly select one player to start, say it’s player two, then moving to player 3, to player 1, and it loops forever. The player starts by discarding cards. The acceptable card type to be discarded include single, double, triple, or quadruple (e.g. single 5, double 6, etc). Then, the next player has to discard cards that’s bigger than the previous user. Order in this game is determined by two factors, value and quantity. When two cards equal in quantity, value is compared; otherwise, always the card with more quantity wins over. As a side note, double joker is considered the biggest of all cards in this game. The detailed ordering used in the game is given as below.
1<2<...<red joker<double 1<double 2<...<double k<triple 1<triple 2<...<triple k<quadruple 1<quadruple 2<...<quadruple k<double joker. 
Whenever one player has no card bigger than the previous, that player may pass (or, the user could just pass even though he/she has bigger cards). When two players pass, then the player discarding the biggest card gets a free move, meaning he/she may start discarding any card he/she wants, so as to start over the loop again. It goes on until one player discards all cards, and that player is the winner.

Operating instruction
When you open the program, you will see a menu with three tabs.
 
Once you enter 1, you will be asked if you want to have an AI joined or user joined
 
Then, your 18 cards will be displayed and you may start the game.
 
When it is your turn to play, you should enter in the form of “x y”, where x is any number between 1 to 4, as to indicate how many number of y you want to discard; and y is any number from 1-10 or ‘J’, ‘Q’, ‘K’, ‘BJOKER’ for black joker, ‘RJOKER’ for red Joker (e.g. “1 4” for single 4, ‘2 S’ for double joker, “3 J” for triple J, etc.). Any input beyond this combination will be treated as invalid, thus returning an error message and asking you to input again.
 
This process goes on and on until one player discards all cards.
If you enter 2 in the menu, an instruction page pops up, explaining the program, then ask you to enter again
 
If you enter 3 in the menus, the program terminates
 

Problem we had
A debugging problem that we had during the design of the program is that, since we have used many dynamically allocated arrays, we accidentally deleted one array while we still needed to use it. It took us a long time to debug this but we eventually found and fixed it.

Challenges we faced
Some challenges that we faced in the design of our program include the designing of the AI. Since the AI is always choosing the card that’s “just” bigger than the previous, and in normal game playing, the user does not get to see the AI’s card (otherwise this game would be meaningless). Therefore, in our testing, we have to develop a separate function that makes AI’s card visible. After testing for many times, we are confident that our AI has our expected property, then we commented this function from our program.

How we hope to improve
For future development, we can make the AI much smarter. For example, if a card single 4 is discarded and the AI has double 5 and single 6, then the AI will manually split the double 5 and discard a single 5, because a single 5 is considered the card “just” bigger than single 4. If it were a smarter AI, it would keep this double 5 and discard a single 6 instead. Also, we could incorporate more interesting rules like having straights and suits taken into consideration. Moreover, the game is largely based on terminal and command line tools. Therefore, if a UI is added, the game will be more interactive. Also, the search algorithm in the game is linear search, binary search might be used to improve the efficiency of the program.

Thank you for reading and have fun playing our game!
