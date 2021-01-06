import React from 'react';
import { PieceChar } from '../ChessEnum';
import style from './ChessBoard/ChessBoard.css';

export default ({ captures, className }) => (
    <div className={style.row}>
        {captures.map((piece, index) => (
            <div key={index} className={`${className} ${style.cellContainer}`}>
                <div className={style.cell}>
                    {PieceChar[piece]}
                </div>
            </div>
        ))}
    </div>
);
