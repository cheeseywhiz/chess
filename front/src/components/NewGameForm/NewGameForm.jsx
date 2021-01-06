import React from 'react';
import { connect } from 'react-redux';
import { Redirect } from 'react-router-dom';
import Form from 'react-bootstrap/Form';
import Button from 'react-bootstrap/Button';
import NewGameFormActions from './redux';

const mapStateToProps = ({ newGameForm }) => ({
    newGameForm,
});

const mapDispatchToProps = (dispatch) => ({
    setPlayer: (player) => dispatch(NewGameFormActions.player.set(player)),
    setOpponent: (opponent) => dispatch(NewGameFormActions.opponent.set(opponent)),
    clearRedirectUrl: () => dispatch(NewGameFormActions.redirectUrl.set()),
    clear: () => dispatch(NewGameFormActions.clear()),
    submit: (player, opponent) => dispatch(NewGameFormActions.submit(player, opponent)),
});

@connect(mapStateToProps, mapDispatchToProps)
export default class extends React.Component {
    componentDidMount() {
        const { clear } = this.props;
        clear();
    }

    render() {
        const {
            newGameForm, setPlayer, setOpponent, clearRedirectUrl, submit,
        } = this.props;

        if (newGameForm.redirectUrl) {
            setTimeout(() => clearRedirectUrl(), 0);
            return <Redirect push to={newGameForm.redirectUrl} />;
        }

        const players = ['White', 'Black', 'Random'];
        return (
            <Form
                noValidate
                onSubmit={(event) => {
                    event.preventDefault();
                    submit(newGameForm.player.value, newGameForm.opponent.value);
                }}
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
                                onChange={() => setPlayer(playerOption)}
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
                        onChange={(event) => {
                            event.preventDefault();
                            setOpponent(event.target.value);
                        }}
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
