package student_player;

import boardgame.Move;

import pentago_twist.PentagoMove;
import pentago_twist.PentagoPlayer;
import pentago_twist.PentagoBoardState;


/** A player file submitted by a student. */
public class StudentPlayer extends PentagoPlayer {

    /**
     * You must modify this constructor to return your student number. This is
     * important, because this is what the code that runs the competition uses to
     * associate you with your agent. The constructor should do nothing else.
     */
    public StudentPlayer() {
        super("260918393");
    }

    /**
     * This is the primary method that you need to implement. The ``boardState``
     * object contains the current state of the game, which your agent must use to
     * make decisions.
     */
    public Move chooseMove(PentagoBoardState boardState) {
        //seizing 2 of the center pieces in the first 4 turns as it increases my chance to win the game.
        if (boardState.getTurnNumber()==0 || boardState.getTurnNumber()==1){
            PentagoMove move = new PentagoMove(1,1,1,1,boardState.getTurnPlayer());
            if (boardState.isLegal(move)){
                return move;
            }
            move = new PentagoMove(1,4,1,1,boardState.getTurnPlayer());
            if (boardState.isLegal(move)){
                return move;
            }
            move = new PentagoMove(4,1,1,1,boardState.getTurnPlayer());
            if (boardState.isLegal(move)){
                return move;
            }
            move = new PentagoMove(4,4,1,1,boardState.getTurnPlayer());
            if (boardState.isLegal(move)){
                return move;
            }
            return MyTools.bestMove(boardState);
        }else{
            return MyTools.bestMove(boardState);
        }
    }
}