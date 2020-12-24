import React from 'react';
import PropTypes from 'prop-types';
import {
    Route, Switch, Redirect, withRouter,
} from 'react-router-dom';
import { connect } from 'react-redux';
import Container from 'react-bootstrap/Container';
import * as actions from '../actions';
import selectors from '../selectors';
import Navbar from './Navbar';
import Login from './Login';
import Create from './Create';

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
        // uninitialized?
        if (username === null) return <div>Loading</div>;
        const mainContent = (
            <Switch>
                <Route exact path="/">
                    <div>index</div>
                </Route>
                <Route>
                    <div>404 not found</div>
                </Route>
            </Switch>
        );
        // set up redirect to the page we redirect from if not logged in
        const url = `${location.pathname}${location.search}${location.hash}`;
        let loginRedirectUrl = '/login';

        if (url !== '/') {
            const params = new URLSearchParams();
            params.set('referrer', url);
            loginRedirectUrl += `?${params}`;
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
                            {username ? mainContent : <Redirect to={loginRedirectUrl} />}
                        </Route>
                    </Switch>
                </Container>
            </>
        );
    }
}

App.propTypes = {
    username: PropTypes.string,
    location: PropTypes.shape({
        pathname: PropTypes.string.isRequired,
        search: PropTypes.string.isRequired,
        hash: PropTypes.string.isRequired,
    }).isRequired,
    init: PropTypes.func.isRequired,
};

App.defaultProps = {
    username: null,
};

export default withRouter(connect(mapStateToProps, mapDispatchToProps)(App));
