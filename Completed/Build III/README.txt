      ____                                    ___        __                
     /\  _`\                                 /\_ \    __/\ \               
     \ \ \L\ \    ___      __   __  __     __\//\ \  /\_\ \ \/'\      __   
      \ \ ,  /   / __`\  /'_ `\/\ \/\ \  /'__`\\ \ \ \/\ \ \ , <    /'__`\ 
       \ \ \\ \ /\ \L\ \/\ \L\ \ \ \_\ \/\  __/ \_\ \_\ \ \ \ \\`\ /\  __/ 
        \ \_\ \_\ \____/\ \____ \ \____/\ \____\/\____\\ \_\ \_\ \_\ \____\
         \/_/\/ /\/___/  \/___L\ \/___/  \/____/\/____/ \/_/\/_/\/_/\/____/
                           /\____/                                         
                           \_/__/                                          
                     __  __      ______  ______  ______     
                    /\ \/\ \    /\__  _\/\__  _\/\__  _\    
                    \ \ \ \ \   \/_/\ \/\/_/\ \/\/_/\ \/    
                     \ \ \ \ \     \ \ \   \ \ \   \ \ \    
                      \ \ \_/ \     \_\ \__ \_\ \__ \_\ \__ 
                       \ `\___/     /\_____\/\_____\/\_____\
                        `\/__/      \/_____/\/_____/\/_____/
                                                            
                      Copyright (c) Tristan Eggenberger 2021

Introduction:
This is the largest update to the Roguelike yet, and I've put dozens of hours into making 
this a minimum-viable-product of sorts for this game. This is the foundation for what's to come.

Legend:

    Objects:
    ! = Healing Potion | This randomly heals you between 3 and 5 hitpoints
    $ = Gold | This gives you between 5 and 25 gold pieces

    Enemies:
    B = Bat | This is the simplest enemy in the Roguelike. With 0 defense and only 3 health, dying to this enemy is the saddest way to go.
    S = Snake | Snakes are a significant step up from bats. If you wander to close, they'll charge at you. Be weary!
    G = Goblin | Goblins are the hardest enemy type, with 6 HP and dealing the same damage as the player, they are a force to be reckoned with,

Controls:
    In Menus:
    UP ARROW: Makes the cursor go up, wraps to bottom if selecting the highest option.
    DOWN ARROW: Makes the cursor go down, wraps to top if selecting the lowest option.
    ENTER: Selects current option and goes to that menu.

    Game Start:
    ANY KEY: Any key that's pressed is added to the player name.
    LEFT ARROW: Moves the input cursor left.
    RIGHT ARROW: Moves the input cursor left.
    ENTER: Enters what has been inputted and starts the game.
    ESCAPE: Returns you to the Main Menu.

    In Game:
    UP ARROW: Moves the player north.
    DOWN ARROW: Moves the player south.
    RIGHT ARROW: Moves the player east.
    LEFT ARROW: Moves the player west.
    SPACEBAR: Wait key, player waits one turn.
    R: If the player is standing on a % tile, the player will proceed to the next floor.
    ESCAPE: Opens in-game menu.

Other information:

    If the player moves onto a space occupied by an enemy, the enemy will be attacked.
    If the player moves onto a space occupied by Gold ($) or a Potion (!) the player will consume that item.

    If the player hasn't taken damage in 20 turns, they will naturally regain 1 hitpoint every 10 turns after that.

IMPORTANT!
    This is a pre-release version, and any issues encountered are NOT indicative of the final product.
    If Roguelike.exe fails to execute, please use RoguelikeWL.exe and report this occurance to @Kraken#8165 as quickly as possible.

    The source code is only included so that you, a tester, can see how the game works if you'd like. You do not have permission to
    distribute the source files. If you do distribute the source files, you will be permanently removed as a game tester and forfeit
    the rights to any other development build of the game.