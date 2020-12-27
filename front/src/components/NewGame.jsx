import React from 'react';
import { connect } from 'react-redux';
import Form from 'react-bootstrap/Form';
import Button from 'react-bootstrap/Button';
import * as actions from '../actions';

const mapStateToProps = ({ newGame }) => ({
    newGame,
});

const mapDispatchToProps = (dispatch) => ({
    clear: () => dispatch(actions.newGameClear()),
    playerSet: (player) => dispatch(actions.newGamePlayerSet(player)),
    opponentSet: (event) => {
        event.preventDefault();
        dispatch(actions.newGameOpponentSet(event.target.value));
    },
    submitNewGame: (player, opponent) => (event) => {
        event.preventDefault();
        dispatch(actions.submitNewGame(player, opponent));
    },
});

class NewGame extends React.Component {
    componentDidMount() {
        const { clear } = this.props;
        clear();
    }

    render() {
        const {
            newGame, playerSet, opponentSet, submitNewGame,
        } = this.props;
        const players = ['White', 'Black', 'Random'];
        return (
            <Form noValidate onSubmit={submitNewGame(newGame.player.text, newGame.opponent.text)}>
                <Form.Group>
                    <Form.Label>Play as</Form.Label>
                    {players.map((playerOption, index) => (
                        <Form.Check
                            key={playerOption}
                            type="radio"
                            id={playerOption}
                        >
                            <Form.Check.Input
                                type="radio"
                                isInvalid={newGame.player.invalid}
                                checked={playerOption === newGame.player.text}
                                onChange={() => playerSet(playerOption)}
                            />
                            <Form.Check.Label>{playerOption}</Form.Check.Label>
                            {index === players.length - 1 && (
                                <Form.Control.Feedback type="invalid">
                                    {newGame.player.invalid}
                                </Form.Control.Feedback>
                            )}
                        </Form.Check>
                    ))}
                </Form.Group>
                <Form.Group>
                    <Form.Label>Opponent</Form.Label>
                    <Form.Control
                        type="text"
                        placeholder="Opponent"
                        value={newGame.opponent.text}
                        onChange={opponentSet}
                        isInvalid={newGame.opponent.invalid}
                    />
                    <Form.Control.Feedback type="invalid">
                        {newGame.opponent.invalid}
                    </Form.Control.Feedback>
                </Form.Group>
                <Button type="submit">New Game</Button>
            </Form>
        );
    }
}

export default connect(mapStateToProps, mapDispatchToProps)(NewGame);
