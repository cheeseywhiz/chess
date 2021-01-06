import React from 'react';
import { connect } from 'react-redux';
import Button from 'react-bootstrap/Button';
import ButtonGroup from 'react-bootstrap/ButtonGroup';
import HistoryActions from './redux';

const mapStateToProps = ({ game, history }) => ({
    game, history,
});

const mapDispatchToProps = (dispatch) => ({
    load: (gameId, stateId) => dispatch(HistoryActions.load(gameId, stateId)),
});

export default connect(mapStateToProps, mapDispatchToProps)(
    ({ game, history, load }) => (
        <ButtonGroup>
            <Button
                type="button"
                onClick={() => load(game.gameId, history.prev)}
                disabled={history.prev === null}
            >
                Prev
            </Button>
            <Button
                type="button"
                onClick={() => load(game.gameId, history.next)}
                disabled={history.next === null}
            >
                Next
            </Button>
        </ButtonGroup>
    ),
);
