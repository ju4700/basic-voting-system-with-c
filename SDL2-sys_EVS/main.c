#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

TTF_Font *titleFont;
TTF_Font *menuFont;
TTF_Font *smallerFont;

enum State {
    MAIN_MENU,
    CANDIDATES_MENU
};

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        fprintf(stderr, "SDL_ttf initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_Window *window = SDL_CreateWindow(
        "Electronic Voting System", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 700, 600, SDL_WINDOW_SHOWN);

    if (!window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    titleFont = TTF_OpenFont("nexa.ttf", 50);
    menuFont = TTF_OpenFont("o.ttf", 30);
    smallerFont = TTF_OpenFont("o.ttf", 20);
    if (!titleFont || !menuFont || !smallerFont) {
        fprintf(stderr, "Font loading failed: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    int quit = 0;
    SDL_Event event;
    int selectedMenuItem = 0;
    int selectedCandidateOption = 0;
    enum State currentState = MAIN_MENU;
    const char *menuOptions[] = {"Start Voting", "Candidates"};
    const char *candidateOptions[] = {"Add Candidates", "Remove Candidates", "Edit Candidates"};
    while (!quit) {
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        if (currentState == MAIN_MENU) {
                            selectedMenuItem = (selectedMenuItem - 1 + 2) % 2;
                        } else {
                            selectedCandidateOption = (selectedCandidateOption - 1 + 3) % 3;
                        }
                        break;
                    case SDLK_DOWN:
                        if (currentState == MAIN_MENU) {
                            selectedMenuItem = (selectedMenuItem + 1) % 2;
                        } else {
                            selectedCandidateOption = (selectedCandidateOption + 1) % 3;
                        }
                        break;
                    case SDLK_RETURN:
                        if (currentState == MAIN_MENU) {
                            printf("Selected: %s\n", menuOptions[selectedMenuItem]);

                            if (selectedMenuItem == 1) {
                                currentState = CANDIDATES_MENU;
                            }
                        } else {
                            printf("Selected: %s\n", candidateOptions[selectedCandidateOption]);
                        }
                        break;
                    case SDLK_ESCAPE:
                        if (currentState == CANDIDATES_MENU) {
                            currentState = MAIN_MENU;
                        } else {
                            quit = 1;
                        }
                        break;
                }
            }
        }
        SDL_Color titleTextColor = {255, 255, 255};
        SDL_Color menuTextColor = {200, 200, 200};
        SDL_Color highlightColor = {255, 204, 153};
        SDL_Color smallerTextColor = {150, 150, 150};

        if (currentState == MAIN_MENU) {
            SDL_Surface *titleSurface = TTF_RenderText_Blended(titleFont, "EVS - 4700", titleTextColor);
            SDL_Texture *titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
            int titleX = (700 - titleSurface->w) / 2;
            int titleY = 20;
            SDL_Rect titleRect = {titleX, titleY, titleSurface->w, titleSurface->h};
            SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
            SDL_FreeSurface(titleSurface);
            SDL_DestroyTexture(titleTexture);

            int menuY = titleY + titleSurface->h + 25;
            for (int i = 0; i < 2; ++i) {
                SDL_Surface *menuSurface = TTF_RenderText_Blended(menuFont, menuOptions[i], (i == selectedMenuItem) ? highlightColor : menuTextColor);
                SDL_Texture *menuTexture = SDL_CreateTextureFromSurface(renderer, menuSurface);

                int menuX = (700 - menuSurface->w) / 2;
                SDL_Rect menuRect = {menuX, menuY, menuSurface->w, menuSurface->h};
                SDL_RenderCopy(renderer, menuTexture, NULL, &menuRect);
                SDL_FreeSurface(menuSurface);
                SDL_DestroyTexture(menuTexture);

                menuY += menuSurface->h + 10;
            }

            SDL_Surface *smallerMessageSurface = TTF_RenderText_Blended(smallerFont, "Press 'esc' to quit the program!", smallerTextColor);
            SDL_Texture *smallerMessageTexture = SDL_CreateTextureFromSurface(renderer, smallerMessageSurface);

            int smallerMessageX = (700 - smallerMessageSurface->w) / 2;
            int smallerMessageY = menuY + 330;
            SDL_Rect smallerMessageRect = {smallerMessageX, smallerMessageY, smallerMessageSurface->w, smallerMessageSurface->h};
            SDL_RenderCopy(renderer, smallerMessageTexture, NULL, &smallerMessageRect);

            SDL_FreeSurface(smallerMessageSurface);
            SDL_DestroyTexture(smallerMessageTexture);
        } else if (currentState == CANDIDATES_MENU) {
            SDL_Surface *candidatesTitleSurface = TTF_RenderText_Blended(titleFont, "Candidates", titleTextColor);
            SDL_Texture *candidatesTitleTexture = SDL_CreateTextureFromSurface(renderer, candidatesTitleSurface);

            int candidatesTitleX = (700 - candidatesTitleSurface->w) / 2;
            int candidatesTitleY = 20;
            SDL_Rect candidatesTitleRect = {candidatesTitleX, candidatesTitleY, candidatesTitleSurface->w, candidatesTitleSurface->h};
            SDL_RenderCopy(renderer, candidatesTitleTexture, NULL, &candidatesTitleRect);

            SDL_FreeSurface(candidatesTitleSurface);
            SDL_DestroyTexture(candidatesTitleTexture);

            int candidateMenuY = candidatesTitleY + candidatesTitleSurface->h + 25;

            for (int i = 0; i < 3; ++i) {
                SDL_Surface *candidateSurface = TTF_RenderText_Blended(menuFont, candidateOptions[i], (i == selectedCandidateOption) ? highlightColor : menuTextColor);
                SDL_Texture *candidateTexture = SDL_CreateTextureFromSurface(renderer, candidateSurface);

                int candidateX = (700 - candidateSurface->w) / 2;
                SDL_Rect candidateRect = {candidateX, candidateMenuY, candidateSurface->w, candidateSurface->h};
                SDL_RenderCopy(renderer, candidateTexture, NULL, &candidateRect);

                SDL_FreeSurface(candidateSurface);
                SDL_DestroyTexture(candidateTexture);

                candidateMenuY += candidateSurface->h + 10;
            }

            SDL_Surface *smallerMessageSurface = TTF_RenderText_Blended(smallerFont, "Press 'esc' to get back to the main menu", smallerTextColor);
            SDL_Texture *smallerMessageTexture = SDL_CreateTextureFromSurface(renderer, smallerMessageSurface);

            int smallerMessageX = (700 - smallerMessageSurface->w) / 2;
            int smallerMessageY = candidateMenuY + 280;
            SDL_Rect smallerMessageRect = {smallerMessageX, smallerMessageY, smallerMessageSurface->w, smallerMessageSurface->h};
            SDL_RenderCopy(renderer, smallerMessageTexture, NULL, &smallerMessageRect);

            SDL_FreeSurface(smallerMessageSurface);
            SDL_DestroyTexture(smallerMessageTexture);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    TTF_CloseFont(titleFont);
    TTF_CloseFont(menuFont);
    TTF_CloseFont(smallerFont);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
