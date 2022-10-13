package student_player;
import boardgame.Move;
import boardgame.Board;
import pentago_twist.PentagoBoardState;
import pentago_twist.PentagoMove;




public class MyTools {
    public static int policyHelper(int numSimulation, int agentColor, PentagoBoardState boardState){
        /*If we have 2 moves that gives the same number of win, then we want to pick the one with the most draws.
        Hence, return the number of points instead of the number of wins.
        */
        int points = 0;
        for (int i = 0; i < numSimulation; i++){//i is the simulation number
            PentagoBoardState resetBoard = ((PentagoBoardState)boardState.clone());//used to reset the board after each simulation
            while(boardState.getWinner() == boardgame.Board.NOBODY){//1 vs 1 randomAgent.
                PentagoMove randMove = (PentagoMove) boardState.getRandomMove();
                boardState.processMove(randMove);
            }
            //Game is over
            if (boardState.getWinner() == agentColor){//check if we won
                points += 1;
            }else{
                if (boardState.getWinner() == Board.DRAW) {//check if it is a draw
                    points += 0;
                }else{//we lost if it isn't a win or a draw...
                    points -= 1;
                }
            }
            boardState = resetBoard;
        }
        return points;
    }

    public static Move bestMove(PentagoBoardState boardState){
        int agentColor = boardState.getTurnPlayer();
        int numSimulation;//number of simulation
        int bestMovePoints;//pretend that the best move lost all simulation to make the update easier
        //int oppColor = 1 - agentColor;
        if(boardState.getTurnNumber()<3){
            numSimulation=100;
            bestMovePoints=-100;
        } else if(boardState.getTurnNumber()==3){
            numSimulation=150;
            bestMovePoints=-150;
        }else{
            numSimulation=200;
            bestMovePoints=-200;
        }
        //System.out.println(boardState.getTurnNumber());
        //System.out.println(numSimulation);

        //PentagoMove blockMove = null;
        PentagoMove bestMove = null;

//Failed attempt to block ennemie move. Either ends up being a suicide move or a useless move as the move I process will affect opponent's next move....
//        for (PentagoMove curOppMove : boardState.getAllLegalMoves()){//loop thorugh all legal move once to see if we will lose in 1 move
//            PentagoBoardState oppBoard = ((PentagoBoardState)boardState.clone());//clone to not modify the real board
//            oppBoard.processMove(curOppMove);
//            if(oppBoard.getWinner() == oppColor){
//                blockMove = curOppMove;//if there is a move that can make the opponent win then we have to save it
//            }if(oppBoard.getWinner() == agentColor){
//                return curOppMove;
//            }
//        }
//        if(blockMove != null) {
//            return blockMove;//block the move if no winning move was found.
//        }

        for (PentagoMove curMove : boardState.getAllLegalMoves()){//try all the possible legal moves
            PentagoBoardState simBoard = ((PentagoBoardState)boardState.clone());//clone to not modify the real board
            simBoard.processMove(curMove);
            if (simBoard.gameOver()){//check if game is over.
                if(simBoard.getWinner() == agentColor){
                    return curMove;
                }else if (simBoard.getWinner() == Board.DRAW){//see if it's a draw.
                    if (bestMovePoints < 0){//a draw is better than a lose. So it will remain as the best move as long as no winning move is found
                        bestMove = curMove;
                        bestMovePoints = numSimulation;//force the draw as it is better to draw then lose. We didn't have a move that guaranteed win...
                        //System.out.println("Saved");
                    }
                }
            }else{//No move can end the game...
                int points = policyHelper(numSimulation, agentColor, simBoard);//finding the best move using the policyHelper
                //System.out.println(points);
                if (points > bestMovePoints){
                    bestMovePoints = points;
                    bestMove = curMove;
                    //System.out.println("Found a move");
                }
            }
        }

        return bestMove;//if no bestMove was found, then return the random move which was initialized at the beginning...
    }
}