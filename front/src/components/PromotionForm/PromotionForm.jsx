import React from 'react';
import { connect } from 'react-redux';
import Form from 'react-bootstrap/Form';
import Button from 'react-bootstrap/Button';
import ButtonGroup from 'react-bootstrap/ButtonGroup';
import PromotionFormActions from './redux';
import GameActions from '../Game/redux';
import { Piece } from '../../ChessEnum';

const mapStateToProps = ({ game, promotionForm }) => ({
    game, promotionForm,
});

const mapDispatchToProps = (dispatch) => ({
    setPiece: (piece) => dispatch(PromotionFormActions.piece.set(piece)),
    promote: (gameId, piece) => dispatch(PromotionFormActions.promote(gameId, piece)),
    clear: () => dispatch(PromotionFormActions.clear()),
    cancel: (gameId) => dispatch(GameActions.init(gameId)),
});

class PromotionForm extends React.Component {
    componentDidMount() {
        const { clear } = this.props;
        clear();
    }

    render() {
        const {
            game, promotionForm,
            promote, cancel, setPiece,
        } = this.props;
        return (
            <Form>
                <Form.Group>
                    <Form.Control
                        as="select"
                        value={promotionForm.piece}
                        onChange={(event) => setPiece(event.target.value)}
                    >
                        <option value="" disabled>Promote to:</option>
                        {[Piece.Knight, Piece.Bishop, Piece.Rook, Piece.Queen].map((piece) => (
                            <option key={piece} value={piece}>{piece}</option>
                        ))}
                    </Form.Control>
                </Form.Group>
                <ButtonGroup>
                    <Button
                        type="button"
                        variant="danger"
                        onClick={() => cancel(game.gameId)}
                    >
                        Cancel
                    </Button>
                    <Button
                        type="submit"
                        onClick={(event) => {
                            event.preventDefault();
                            if (!promotionForm.piece) return;
                            promote(game.gameId, promotionForm.piece);
                        }}
                    >
                        Promote
                    </Button>
                </ButtonGroup>
            </Form>
        );
    }
}

export default connect(mapStateToProps, mapDispatchToProps)(PromotionForm);
