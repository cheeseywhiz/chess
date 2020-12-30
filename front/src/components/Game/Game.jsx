import React from 'react';
import { withRouter } from 'react-router-dom';
import { connect } from 'react-redux';
import GameActions from './redux';
import LoadingSpinner from '../LoadingSpinner';

const mapStateToProps = ({ game }) => ({
    game,
});

const mapDispatchToProps = (dispatch) => ({
    init: (gameId) => dispatch(GameActions.init(gameId)),
});

class Game extends React.Component {
    componentDidMount() {
        const { location, init } = this.props;
        const params = new URLSearchParams(location.search);
        const gameId = params.get('gameId');
        if (gameId === null) throw new Error('no gameId in url');
        init(gameId);
    }

    render() {
        const { game } = this.props;
        const { gameId } = game;
        if (gameId === null) return <LoadingSpinner />;
        return <code>{JSON.stringify(game)}</code>;
    }
}

export default withRouter(connect(mapStateToProps, mapDispatchToProps)(Game));
