import React from 'react';
import {
    Route, Switch, Redirect, withRouter,
} from 'react-router-dom';
import { connect } from 'react-redux';
import Container from 'react-bootstrap/Container';
import LoginFormActions from './LoginForm/redux';
import LoadingSpinner from './LoadingSpinner';
import Navbar from './Navbar/Navbar';
import LoginForm from './LoginForm/LoginForm';
import CreateForm from './CreateForm/CreateForm';
import NewGameForm from './NewGameForm/NewGameForm';
import Game from './Game/Game';
import UserGames from './UserGames/UserGames';

const mapStateToProps = ({ username }) => ({
    username,
});

const mapDispatchToProps = (dispatch) => ({
    login: () => dispatch(LoginFormActions.login()),
});

@withRouter
@connect(mapStateToProps, mapDispatchToProps)
export default class extends React.Component {
    componentDidMount() {
        const { login } = this.props;
        login();
    }

    render() {
        const { username, location } = this.props;
        // set up redirect to the page we redirect from if not logged in
        const url = `${location.pathname}${location.search}${location.hash}`;
        let loginRedirectUrl = '/login';

        if (url !== '/') {
            const params = new URLSearchParams();
            params.set('referrer', url);
            loginRedirectUrl += `?${params}`;
        }

        let authenticatedContent;

        if (username === null) {
            authenticatedContent = <LoadingSpinner />;
        } else if (username) {
            authenticatedContent = (
                <Switch>
                    <Route exact path="/">
                        <div>index</div>
                    </Route>
                    <Route exact path="/new_game">
                        <NewGameForm />
                    </Route>
                    <Route exact path="/game">
                        <Game />
                    </Route>
                    <Route exact path="/games">
                        <UserGames />
                    </Route>
                    <Route>
                        <div>404 not found</div>
                    </Route>
                </Switch>
            );
        } else {
            authenticatedContent = <Redirect to={loginRedirectUrl} />;
        }

        return (
            <>
                <Navbar />
                <Container>
                    <Switch>
                        <Route exact path="/login">
                            <LoginForm />
                        </Route>
                        <Route exact path="/create">
                            <CreateForm />
                        </Route>
                        <Route>
                            {authenticatedContent}
                        </Route>
                    </Switch>
                </Container>
            </>
        );
    }
}
