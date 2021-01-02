import React from 'react';
import { connect } from 'react-redux';
import Container from 'react-bootstrap/Container';
import { Player } from '../../ChessEnum';
import Circle from '../Circle';
import style from './ChessBoard.css';
import ChessBoardActions from './redux';

const mapStateToProps = ({ game, chessBoard }) => {
    const { gameId, state } = game;
    const { board } = state;
    console.assert(board !== null, 'rendering ChessBoard without a board in the state');
    return { gameId, board, chessBoard };
};

const mapDispatchToProps = (dispatch) => ({
    getMoves: (gameId, row, col) => dispatch(ChessBoardActions.getMoves(gameId, row, col)),
    clear: () => {
        dispatch(ChessBoardActions.moves.clear());
        dispatch(ChessBoardActions.selected.clear());
    },
});

const PieceChar = {
    None: <>&nbsp;</>,
    Pawn: 'p',
    Knight: 'N',
    Bishop: 'B',
    Rook: 'R',
    Queen: 'Q',
    King: 'K',
};

const movesContains = (moves, row, col) => {
    for (let i = 0; i < moves.length; i += 1) {
        if (moves[i][0] === row && moves[i][1] === col) return true;
    }

    return false;
};

class ChessBoard extends React.Component {
    componentDidMount() {
        const { clear } = this.props;
        clear();
    }

    getCell([piece, player], rowIndex, colIndex) {
        const {
            gameId, chessBoard,
            getMoves, clear,
        } = this.props;
        let bg;

        if (chessBoard.selected.row === rowIndex && chessBoard.selected.col === colIndex) {
            bg = 'bg-warning';
        } else if ((rowIndex + colIndex) % 2 === 0) {
            bg = 'bg-primary';
        } else {
            bg = 'bg-secondary';
        }

        const classNames = [
            style.cellContainer, bg,
            player === Player.White ? 'text-white' : 'text-dark',
        ];
        const move = movesContains(chessBoard.moves, rowIndex, colIndex) ? (
            <div className={style.overlay}>
                <Circle />
            </div>
        ) : null;
        return (
            <div
                key={colIndex}
                className={classNames.join(' ')}
                role="button"
                tabIndex={-1}
                onClick={() => {
                    if (chessBoard.selected.row === rowIndex
                            && chessBoard.selected.col === colIndex) {
                        clear();
                    } else {
                        getMoves(gameId, rowIndex, colIndex);
                    }
                }}
                onKeyDown={({ key }) => {
                    if (key === 'Escape') clear();
                }}
            >
                <div className={style.cell}>
                    <div>
                        {PieceChar[piece]}
                    </div>
                </div>
                {move}
            </div>
        );
    }

    render() {
        const { board } = this.props;
        return (
            <div className={style.boardContainer}>
                <Container className={style.board}>
                    {board.map((row, rowIndex) => (
                        <div key={rowIndex} className={style.row}>
                            {row.map((cell, colIndex) => (
                                this.getCell(cell, rowIndex, colIndex)
                            ))}
                        </div>
                    ))}
                </Container>
            </div>
        );
    }
}

export default connect(mapStateToProps, mapDispatchToProps)(ChessBoard);
