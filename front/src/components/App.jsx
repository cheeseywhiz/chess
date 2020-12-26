import React from 'react';
import {
    Route, Switch, Redirect, withRouter,
} from 'react-router-dom';
import { connect } from 'react-redux';
import Container from 'react-bootstrap/Container';
import * as actions from '../actions';
import selectors from '../selectors';
import LoadingSpinner from './LoadingSpinner';
import Navbar from './Navbar';
import Login from './Login';
import Create from './Create';
import NewGame from './NewGame';

const mapStateToProps = (state) => ({
    username: selectors.username(state),
});

const mapDispatchToProps = (dispatch) => ({
    init: () => dispatch(actions.init()),
});

class App extends React.Component {
    componentDidMount() {
        const { init } = this.props;
        init();
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
                        <NewGame />
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
                            <Login />
                        </Route>
                        <Route exact path="/create">
                            <Create />
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

export default withRouter(connect(mapStateToProps, mapDispatchToProps)(App));
