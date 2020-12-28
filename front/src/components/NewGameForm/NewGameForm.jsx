import React from 'react';
import { connect } from 'react-redux';
import Form from 'react-bootstrap/Form';
import Button from 'react-bootstrap/Button';
import NewGameFormActions from './redux';

const mapStateToProps = ({ newGameForm }) => ({
    newGameForm,
});

const mapDispatchToProps = (dispatch) => ({
    clear: () => dispatch(NewGameFormActions.clear()),
    playerSet: (player) => dispatch(NewGameFormActions.player.set(player)),
    opponentSet: (event) => {
        event.preventDefault();
        dispatch(NewGameFormActions.opponent.set(event.target.value));
    },
    submitNewGame: (player, opponent) => (event) => {
        event.preventDefault();
        dispatch(NewGameFormActions.submit(player, opponent));
    },
});

class NewGameForm extends React.Component {
    componentDidMount() {
        const { clear } = this.props;
        clear();
    }

    render() {
        const {
            newGameForm, playerSet, opponentSet, submitNewGame,
        } = this.props;
        const players = ['White', 'Black', 'Random'];
        return (
            <Form
                noValidate
                onSubmit={submitNewGame(
                    newGameForm.player.value, newGameForm.opponent.value,
                )}
            >
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
                                isInvalid={newGameForm.player.invalid}
                                checked={playerOption === newGameForm.player.value}
                                onChange={() => playerSet(playerOption)}
                            />
                            <Form.Check.Label>{playerOption}</Form.Check.Label>
                            {index === players.length - 1 && (
                                <Form.Control.Feedback type="invalid">
                                    {newGameForm.player.invalid}
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
                        value={newGameForm.opponent.value}
                        onChange={opponentSet}
                        isInvalid={newGameForm.opponent.invalid}
                    />
                    <Form.Control.Feedback type="invalid">
                        {newGameForm.opponent.invalid}
                    </Form.Control.Feedback>
                </Form.Group>
                <Button type="submit">New Game</Button>
            </Form>
        );
    }
}

export default connect(mapStateToProps, mapDispatchToProps)(NewGameForm);
