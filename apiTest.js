var window = SDL3.CreateWindow("My SDL3 Window from SimpleJS program", 640, 480, 0);
var renderer = SDL3.CreateRenderer(window, "My SDL3 Renderer");

var running = true;
var event = SDL3.CreateEvent();

var max_c = 255;
var r = 50, g = 100, b = 50;
var dr = 1, dg = 1, db = 1;

SDL3.SetRenderVSync(renderer, SDL3.SDL_RENDERER_VSYNC_ENABLED);

function abs(a)
{
    if (a < 0)
        return a * -1;

    return a;
}

function makeCircle(v, max)
{
    return abs(max - (v % (max * 2)));
}

var start, end, diff;

while (running) {
    while (SDL3.PollEvent(event)) {
        if (event.type == SDL3.SDL_EVENT_QUIT) {
            running = false;
        }
    }

    start = end;
    end = SDL3.GetTicksNS() / 1000000000;
    diff = end - start;

    var cr = makeCircle(r, max_c);
    var cg = makeCircle(g, max_c);
    var cb = makeCircle(b, max_c);

    r += ((cg + 1) / 99.325) * diff * 60;
    g += ((cb + 5) / 88.654) * diff * 60;
    b += ((cr + 1) / 132.40) * diff * 60;

    SDL3.SetRenderDrawColor(renderer, makeCircle(r, max_c), makeCircle(g, max_c), makeCircle(b, max_c), 255);
    SDL3.RenderClear(renderer);

    SDL3.RenderPresent(renderer);

    SDL3.Print(1 / diff);
}

SDL3.DestroyRenderer(renderer);
SDL3.DestroyWindow(window);
