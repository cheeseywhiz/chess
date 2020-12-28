// fetch with improved interface
// cookies are included by default
// option json sets body, of course, Content-Type header, and sets default method to POST
// see https://developer.mozilla.org/en-US/docs/Web/API/Response#An_Ajax_Call for ajax inspiration
export default async ({ url, json, ...optionsIn }) => {
    let options = { credentials: 'same-origin', headers: {}, ...optionsIn };

    if (json !== undefined) {
        options = { method: 'post', ...options, body: JSON.stringify(json) };
        options.headers['Content-Type'] = 'application/json';
    }

    const response = await fetch(url, options);
    if (response.status >= 500) throw new Error(response.statusText);
    if (response.status === 204) return Promise.resolve();
    const jsonRes = await response.json();

    if (response.status >= 400) {
        console.log(jsonRes);
        return Promise.reject(jsonRes);
    }

    return Promise.resolve(jsonRes);
};
