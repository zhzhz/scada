//深拷贝
function deepCopy(old, newRrray)
{
    var c = newRrray || {};
    for (var i in old)
    {
        if (typeof old[i] === 'object')
        {
            c[i] = (old[i].constructor === Array) ? [] : {};
            deepCopy(old[i], c[i]);
        }
        else
        {
            c[i] = old[i];
        }
    }
    return c;
}
