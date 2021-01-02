const path = require('path');
const HtmlWebpackPlugin = require('html-webpack-plugin');

const STATIC = path.join(__dirname, 'static');
const SOURCE = path.join(__dirname, 'src');

module.exports = {
    mode: 'development',
    entry: './src/main.jsx',
    output: {
        path: STATIC,
        filename: 'bundle.js',
    },
    plugins: [
        new HtmlWebpackPlugin({
            template: path.join(SOURCE, 'index.html'),
            title: 'Chess',
        }),
    ],
    module: {
        rules: [
            {
                test: /\.jsx?$/,
                loader: 'babel-loader',
                options: {
                    presets: [
                        '@babel/preset-env',
                        '@babel/preset-react',
                    ],
                },
            },
            {
                test: /\.css$/,
                use: [
                    {
                        loader: 'style-loader',
                        options: {
                            injectType: 'singletonStyleTag',
                        },
                    },
                    {
                        loader: 'css-loader',
                        options: {
                            modules: {
                                localIdentName: '[name]--[local]--[hash:hex:5]',
                            },
                        },
                    },
                ],
            },
        ],
    },
    resolve: {
        extensions: ['.js', '.jsx'],
    },
};
