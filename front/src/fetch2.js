// fetch with improved interface
// cookies are included by default
// option params builds URLSearchParams
// option json sets body, of course, Content-Type header, and sets default method to POST
// see https://developer.mozilla.org/en-US/docs/Web/API/Response#An_Ajax_Call for ajax inspiration
export default async ({
    url, params, json, ...optionsIn
}) => {
    let options = { credentials: 'same-origin', headers: {}, ...optionsIn };

    let url2 = url;

    if (params !== undefined) {
        const urlParams = new URLSearchParams();
        Object.entries(params).forEach(([key, value]) => urlParams.set(key, value));
        url2 += `?${urlParams}`;
    }

    if (json !== undefined) {
        options = { method: 'post', ...options, body: JSON.stringify(json) };
        options.headers['Content-Type'] = 'application/json';
    }

    const response = await fetch(url2, options);

    if (response.status >= 500) {
        const fakeRes = {
            status: response.status,
            message: response.statusText,
            reason: 'server error',
        };
        return Promise.reject(fakeRes);
    }

    if (response.status === 204) return Promise.resolve();
    const jsonRes = await response.json();

    if (response.status >= 400) {
        console.log(jsonRes);
        return Promise.reject(jsonRes);
    }

    return Promise.resolve(jsonRes);
};
