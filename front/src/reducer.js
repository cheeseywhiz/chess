import { combineReducers } from 'redux';
import { reducer as UsernameReducer } from './components/Navbar/UsernameRedux';
import { reducer as LoginFormReducer } from './components/LoginForm/redux';
import { reducer as CreateFormReducer } from './components/CreateForm/redux';
import { reducer as NewGameFormReducer } from './components/NewGameForm/redux';
import { reducer as GameReducer } from './components/Game/redux';

export default combineReducers({
    username: UsernameReducer,
    loginForm: LoginFormReducer,
    createForm: CreateFormReducer,
    newGameForm: NewGameFormReducer,
    game: GameReducer,
});
