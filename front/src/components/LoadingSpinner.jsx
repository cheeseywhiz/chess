import React from 'react';
import Container from 'react-bootstrap/Container';

export default () => (
    <Container>
        <div className="d-flex justify-content-center">
            <div className="spinner-border" role="status">
                <div className="sr-only">
                    Loading...
                </div>
            </div>
        </div>
    </Container>
);
