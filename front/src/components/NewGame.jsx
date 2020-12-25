import React from 'react';
import { connect } from 'react-redux';
import Form from 'react-bootstrap/Form';
import Button from 'react-bootstrap/Button';
import selectors from '../selectors';
import * as actions from '../actions';

const mapStateToProps = (state) => ({
    player: selectors.newGame.player(state),
    opponent: selectors.newGame.opponent(state),
});

const mapDispatchToProps = (dispatch) => ({
    playerSet: (player) => dispatch(actions.newGamePlayerSet(player)),
    opponentSet: (event) => {
        event.preventDefault();
        dispatch(actions.newGameOpponentSet(event.target.value));
    },
});

export default connect(mapStateToProps, mapDispatchToProps)(
    ({
        player, opponent, playerSet, opponentSet,
    }) => {
        const players = ['White', 'Black', 'Random'];
        return (
            <Form onSubmit={(event) => event.preventDefault()}>
                <Form.Group>
                    <Form.Label>Play as</Form.Label>
                    {players.map((playerOption) => (
                        <Form.Check
                            type="radio"
                            key={playerOption}
                            id={playerOption}
                            label={playerOption}
                            checked={playerOption === player}
                            onChange={() => playerSet(playerOption)}
                        />
                    ))}
                </Form.Group>
                <Form.Group>
                    <Form.Label>Opponent</Form.Label>
                    <Form.Control
                        type="text"
                        placeholder="Opponent"
                        value={opponent}
                        onChange={opponentSet}
                    />
                </Form.Group>
                <Button type="submit">New Game</Button>
            </Form>
        );
    },
);
