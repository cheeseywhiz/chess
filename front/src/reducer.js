import { combineReducers } from 'redux';
import { reducer as UsernameReducer } from './components/Navbar/UsernameRedux';
import { reducer as LoginFormReducer } from './components/LoginForm/redux';
import { reducer as CreateFormReducer } from './components/CreateForm/redux';
import { reducer as NewGameFormReducer } from './components/NewGameForm/redux';
import { reducer as GameReducer } from './components/Game/redux';
import { reducer as ChessBoardReducer } from './components/ChessBoard/redux';
import { reducer as PromotionFormReducer } from './components/PromotionForm/redux';
import { reducer as HistoryReducer } from './components/History/redux';

export default combineReducers({
    username: UsernameReducer,
    loginForm: LoginFormReducer,
    createForm: CreateFormReducer,
    newGameForm: NewGameFormReducer,
    game: GameReducer,
    chessBoard: ChessBoardReducer,
    promotionForm: PromotionFormReducer,
    history: HistoryReducer,
});
