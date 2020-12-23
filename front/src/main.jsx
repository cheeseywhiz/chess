import React from 'react';
import ReactDOM from 'react-dom';
import {
    BrowserRouter as Router, Route, Switch,
} from 'react-router-dom';

const Main = () => (
    <Router>
        <Switch>
            <Route exact path="/">
                <div>index</div>
            </Route>
            <Route>
                <div>404 not found</div>
            </Route>
        </Switch>
    </Router>
);

ReactDOM.render(<Main />, document.getElementById('react-app'));
