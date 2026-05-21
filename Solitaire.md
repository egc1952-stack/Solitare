Solitare

Goal:
    To enermurate all possible moves with a deal. There are two main methods:
    Recursively determin all plays until the entire set is exhausted and then record the results.
    Recieve an interim layout and compile all the possible plays using that layout, recording each result. At the end of that layout, the program is done. This is boinc-esq in that tasks related to previouse results are issued to a queue where the program fetched a task to work on. This would benefit from multi-threading. It also has the benfit of identifying a task that has be previously processed so that it can just be added to the completed tasks for that game.

General design:
    Parent process will start a game by suffleing a deck and dealing out the cards to the table.
    Parent then loads this inital deck to the task queue
    Parent then recived the output from a child process that contains the resulting pylay on the task the child is / was working on
    Parent the integrates this into the result database ensuring that there are no duplicates.
    The child's task is then appended to the task queue.
    Parent will create new child threads as needed

    