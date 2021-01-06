import React from 'react';
import { withRouter } from 'react-router-dom';
import { connect } from 'react-redux';
import GameActions from './redux';
import LoadingSpinner from '../LoadingSpinner';
import ChessBoard from '../ChessBoard/ChessBoard';
import PromotionForm from '../PromotionForm/PromotionForm';
import History from '../History/History';
import { Player, State, EndgameState } from '../../ChessEnum';

const mapStateToProps = ({ username, game }) => ({
    username,
    game,
});

const mapDispatchToProps = (dispatch) => ({
    init: (gameId) => dispatch(GameActions.init(gameId)),
});

@withRouter
@connect(mapStateToProps, mapDispatchToProps)
export default class extends React.Component {
    componentDidMount() {
        const { location, init } = this.props;
        const params = new URLSearchParams(location.search);
        const gameId = params.get('gameId');
        if (gameId === null) throw new Error('no gameId in url');
        init(gameId);
    }

    render() {
        const { username, game } = this.props;
        const { gameId } = game;
        if (gameId === null) return <LoadingSpinner />;
        const { white, black, state } = game;
        const { player } = state;
        let playerUsername;
        let opponentUsername;
        let opponent;

        if (player === Player.White) {
            playerUsername = white;
            opponentUsername = black;
            opponent = Player.Black;
        } else {
            playerUsername = black;
            opponentUsername = white;
            opponent = Player.White;
        }

        let turn;

        if (playerUsername === username) {
            switch (game.state.endgameState) {
            case EndgameState.None:
                turn = `Your (${player}) turn against ${opponentUsername} (${opponent})`;
                break;
            case EndgameState.Check:
                turn = `${opponentUsername} (${opponent}) is in check against you (${player})`;
                break;
            case EndgameState.Checkmate:
                turn = `${opponentUsername} (${opponent}) has checkmated you (${player})!`;
                break;
            case EndgameState.Stalemate:
                turn = `${opponentUsername} (${opponent}) has stalemated against you (${player})`;
                break;
            default:
                break;
            }
        } else {
            switch (game.state.endgameState) {
            case EndgameState.None:
                turn = `${playerUsername}'s (${player}) turn against you (${opponent})`;
                break;
            case EndgameState.Check:
                turn = `You (${opponent}) are in check against ${playerUsername} (${player})`;
                break;
            case EndgameState.Checkmate:
                turn = `You (${opponent}) have checkmated ${playerUsername} (${player})!`;
                break;
            case EndgameState.Stalemate:
                turn = `You (${opponent}) have stalemated ${playerUsername} (${player})`;
                break;
            default:
                break;
            }
        }

        return (
            <>
                <ChessBoard />
                <History />
                {game.state.state === State.Promotion ? <PromotionForm /> : null}
                <div>
                    {'Move '}
                    {game.state.nMoves}
                </div>
                <div>
                    {turn}
                </div>
            </>
        );
    }
}
