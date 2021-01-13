import React from 'react';
import { connect } from 'react-redux';
import { Link } from 'react-router-dom';
import ListGroup from 'react-bootstrap/ListGroup';
import UserGamesActions from './redux';

const mapStateToProps = ({ userGames }) => ({
    userGames,
});

const mapDispatchToProps = (dispatch) => ({
    load: () => dispatch(UserGamesActions.games.load()),
});

@connect(mapStateToProps, mapDispatchToProps)
export default class extends React.Component {
    componentDidMount() {
        const { load } = this.props;
        load();
    }

    render() {
        const { userGames } = this.props;
        return (
            <>
                <ListGroup>
                    {userGames.games.map(({ white, black, gameId }) => (
                        <ListGroup.Item key={gameId}>
                            <ul>
                                <li>
                                    gameId
                                    <ul><li>{gameId}</li></ul>
                                </li>
                                <li>
                                    <Link to={`/game?gameId=${gameId}`}>link</Link>
                                </li>
                                <li>
                                    white
                                    <ul><li>{white}</li></ul>
                                </li>
                                <li>
                                    black
                                    <ul><li>{black}</li></ul>
                                </li>
                            </ul>
                        </ListGroup.Item>
                    ))}
                </ListGroup>
            </>
        );
    }
}
