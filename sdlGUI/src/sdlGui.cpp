#include <vector>

#include <mpd/connection.h>
#include <mpd/error.h>
#include <mpd/mixer.h>
#include <mpd/player.h>
#include <mpd/playlist.h>
#include <mpd/queue.h>
#include <mpd/status.h>


#include <mpd/client.h>
#include <mpd/status.h>
#include <mpd/entity.h>
#include <mpd/search.h>
#include <mpd/tag.h>
#include <mpd/message.h>


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>





//	void test() {
//		mpd_run_playlist_add(connection, name, path)
//
//		mpd_playlist_begin(pair)
//		mpd_playlist_free(struct mpd_playlist *playlist);
//
//
//		struct mpd_status * status;
//			struct mpd_song *song;
//			const struct mpd_audio_format *audio_format;
//
//			mpd_command_list_begin(conn, true);
//			mpd_send_status(conn);
//			mpd_send_current_song(conn);
//			mpd_command_list_end(conn);
//
//			status = mpd_recv_status(conn);
//			if (status == NULL)
//				return handle_error(conn);
//
//			printf("volume: %i\n", mpd_status_get_volume(status));
//			printf("repeat: %i\n", mpd_status_get_repeat(status));
//			printf("queue version: %u\n", mpd_status_get_queue_version(status));
//			printf("queue length: %i\n", mpd_status_get_queue_length(status));
//			if (mpd_status_get_error(status) != NULL)
//				printf("error: %s\n", mpd_status_get_error(status));
//
//			if (mpd_status_get_state(status) == MPD_STATE_PLAY ||
//			    mpd_status_get_state(status) == MPD_STATE_PAUSE) {
//				printf("song: %i\n", mpd_status_get_song_pos(status));
//				printf("elaspedTime: %i\n",mpd_status_get_elapsed_time(status));
//				printf("elasped_ms: %u\n", mpd_status_get_elapsed_ms(status));
//				printf("totalTime: %i\n", mpd_status_get_total_time(status));
//				printf("bitRate: %i\n", mpd_status_get_kbit_rate(status));
//			}
//
//			audio_format = mpd_status_get_audio_format(status);
//			if (audio_format != NULL) {
//				printf("sampleRate: %i\n", audio_format->sample_rate);
//				printf("bits: %i\n", audio_format->bits);
//				printf("channels: %i\n", audio_format->channels);
//			}
//
//			mpd_status_free(status);
//
//			if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS)
//				return handle_error(conn);
//
//			mpd_response_next(conn);
//
//			while ((song = mpd_recv_song(conn)) != NULL) {
//				printf("uri: %s\n", mpd_song_get_uri(song));
//				print_tag(song, MPD_TAG_ARTIST, "artist");
//				print_tag(song, MPD_TAG_ALBUM, "album");
//				print_tag(song, MPD_TAG_TITLE, "title");
//				print_tag(song, MPD_TAG_TRACK, "track");
//				print_tag(song, MPD_TAG_NAME, "name");
//				print_tag(song, MPD_TAG_DATE, "date");
//
//				if (mpd_song_get_duration(song) > 0) {
//					printf("time: %u\n", mpd_song_get_duration(song));
//				}
//
//				printf("pos: %u\n", mpd_song_get_pos(song));
//
//				mpd_song_free(song);
//			}
//
//			if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS ||
//			    !mpd_response_finish(conn))
//				return handle_error(conn);
//
//		struct mpd_entity * entity;
//
//				if (!mpd_send_list_meta(conn, argv[2]))
//					return handle_error(conn);
//
//				while ((entity = mpd_recv_entity(conn)) != NULL) {
//					const struct mpd_song *song;
//					const struct mpd_directory *dir;
//					const struct mpd_playlist *pl;
//
//					switch (mpd_entity_get_type(entity)) {
//					case MPD_ENTITY_TYPE_UNKNOWN:
//						break;
//
//					case MPD_ENTITY_TYPE_SONG:
//						song = mpd_entity_get_song(entity);
//						printf("uri: %s\n", mpd_song_get_uri(song));
//						print_tag(song, MPD_TAG_ARTIST, "artist");
//						print_tag(song, MPD_TAG_ALBUM, "album");
//						print_tag(song, MPD_TAG_TITLE, "title");
//						print_tag(song, MPD_TAG_TRACK, "track");
//						break;
//
//					case MPD_ENTITY_TYPE_DIRECTORY:
//						dir = mpd_entity_get_directory(entity);
//						printf("directory: %s\n", mpd_directory_get_path(dir));
//						break;
//
//					case MPD_ENTITY_TYPE_PLAYLIST:
//						pl = mpd_entity_get_playlist(entity);
//						printf("playlist: %s\n",
//						       mpd_playlist_get_path(pl));
//						break;
//					}
//
//					mpd_entity_free(entity);
//				}
//
//				if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS ||
//				    !mpd_response_finish(conn))
//					return handle_error(conn);
//	}
//
//	static void
//	print_tag(const struct mpd_song *song, enum mpd_tag_type type,
//		  const char *label)
//	{
//		unsigned i = 0;
//		const char *value;
//
//		while ((value = mpd_song_get_tag(song, type, i++)) != NULL)
//			printf("%s: %s\n", label, value);
//	}


// Liste der Songs:
//struct mpd_playlist *
//mpd_recv_playlist(struct mpd_connection *connection);

const int numCommands = 13;


SDL_Color backgroundNight = { 0,0,50,255 };
SDL_Color backgroundDay = {255,255,255,255};


class Application {
private:
	bool night;
	int playlist;
	int dayPlaylist;

	SDL_Event userEvent;
	int pressedButton = -1;
	SDL_Window * window ;
	SDL_Renderer * renderer;
	SDL_Texture* texture[numCommands];

	bool shuffleDefault;

	std::string hostname = "127.0.0.1";

	std::string tracklist[5];
	float trackProgress;
	float trackLength;
	int currentSong;

	const char* imgFiles[numCommands] = {
			"playlist1.bmp", "playlist2.bmp", "playlist3.bmp", "playlist4.bmp", "prev.bmp", "play.bmp", "stop.bmp", "next.bmp", "vol_down.bmp", "vol_up.bmp", "death.bmp", "cleaning.bmp", "playlist.bmp"};

	enum BUTTONS {
		PLAYLIST1=0, PLAYLIST2, PLAYLIST3, PLAYLIST4, PREV, PLAY, STOP, NEXT, VOL_DOWN, VOL_UP, DEATH, LAST_BUTTON_IN_TOOLBAR
	};
	enum SEPARATE_BUTTONS {
		CLEANING = LAST_BUTTON_IN_TOOLBAR,
		BUTTON_PLAYLIST,
	};

	enum DISPLAY_MODE {
		CLOCK_MODE,
		PLAYLIST_MODE
	};

	DISPLAY_MODE displayMode;


	struct mpd_connection *conn;

	std::chrono::time_point<std::chrono::system_clock> lastCleanTime;


	TTF_Font* font260;
	TTF_Font* font30;

public:

	// init
	Application(std::vector<std::string> args) {
		{
			bool hostnameSwitch = false;
			for (std::string arg : args) {
				if (arg == "-h") {
					hostnameSwitch = true;
				} else {
					if (hostnameSwitch) {
						hostname = arg;
					}
					hostnameSwitch = false;
				}
			}
		}
		userEvent.type = SDL_USEREVENT;
		srand(time(0));
		conn = 0;
		night = false;
		playlist = -1;
		dayPlaylist = -1;
		shuffleDefault = false;

		displayMode = CLOCK_MODE;

		// init SDL
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
		window = SDL_CreateWindow("MPD Client",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1184, 624, 0);

		renderer = SDL_CreateRenderer(window, -1, 0);

		for (int i = 0; i < numCommands; i++) {
			std::string filename = imgFiles[i];
			SDL_Surface* surface = SDL_LoadBMP(filename.c_str());
			if (!surface) {
				throw std::logic_error("could not load image file: " + filename);
			}
			SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
			texture[i] = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_SetTextureBlendMode(texture[i], SDL_BLENDMODE_BLEND);
			SDL_FreeSurface(surface);
		}


		TTF_Init();

		font260 = TTF_OpenFont("Vera.ttf", 260);
		font30 = TTF_OpenFont("Vera.ttf", 30);

		conn = mpd_connection_new(hostname.c_str(), 0, 30000);

		setPlaylist(4);

		SDL_DisableScreenSaver();
	}

	// shutdown
	~Application() {
		if (conn != 0) {
			mpd_connection_free(conn);
		}

	    // cleanup SDL
	    for (int i=0; i <  numCommands; i++) {
	    	SDL_DestroyTexture(texture[i]);
	    }

	    SDL_DestroyRenderer(renderer);
	    SDL_DestroyWindow(window);
	    TTF_Quit();
	    SDL_Quit();
	}

	void renderText(TTF_Font* font, SDL_Renderer* renderer, std::string text, SDL_Color color, SDL_Point position, bool center) {
		if (text.empty()) {
			return;
		}
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
		if (textSurface == nullptr) {
			return;
		}
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == nullptr) {
			return;
		}
		int textWidth = textSurface->w;
		int textHeight = textSurface->h;
		SDL_FreeSurface(textSurface);

		SDL_Rect rect = {
			position.x,
			position.y,
			textWidth,
			textHeight
		};

		if (center) {
			rect.x -= textWidth / 2;
			rect.y -= textHeight / 2;
		}

		SDL_RenderCopy(renderer, texture, nullptr, &rect);
		SDL_DestroyTexture(texture);
	}

	SDL_Point getTextSize(TTF_Font* font, std::string text) {
		SDL_Point pt;
		TTF_SizeText(font, text.c_str(), &pt.x, &pt.y);
		return pt;
	}

	std::string getTrackName() {
		return tracklist[2];
	}

	float getTrackProgress() {
		return trackProgress;
	}

	std::string getTime() {
		std::time_t result = std::time(nullptr);
		std::tm* local = localtime(&result);
		std::stringstream stream;
		stream << std::setw(2) << std::setfill('0');
		stream << local->tm_hour;
		stream << ":";
		stream << std::setw(2) << std::setfill('0');
		stream << local->tm_min;
		return stream.str();
	}


	std::string getDate() {
		std::time_t result = std::time(nullptr);
		std::tm* local = localtime(&result);

		std::string weekdays[] = { "Sonntech", "Montech", "Dienstech", "Mittwech", "Donnerstech", "Freitech", "Samstech" };

		std::stringstream stream;
		stream << weekdays[local->tm_wday];
		stream << ", ";
		stream << std::setw(2) << std::setfill('0') << local->tm_mday;
		stream << ".";
		stream << std::setw(2) << std::setfill('0') << (local->tm_mon+1);
		stream << ".";
		stream << std::setw(4) << std::setfill('0') << (local->tm_year + 1900);
		return stream.str();
	}

	// main loop
	int run() {
		if (conn == 0) {
			return -1;
		}
		if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS)
		{
			fprintf(stderr, "%s\n", mpd_connection_get_error_message(conn));
			return -1;
		}

	    SDL_Event event;
		bool quit = false;
		 while (!quit) {
			update();
			render();
			if (SDL_WaitEventTimeout(&event, 100)) {
				handleEvent(event, quit);
				while (SDL_PollEvent(&event)) {
					handleEvent(event, quit);
				}
			}
		 }
		 return 0;
	}

	void handleEvent(SDL_Event& event, bool& quit) {
		switch (event.type)
		{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN: {
				SDL_Point point = { event.button.x, event.button.y };

				// check for button press
				{
					pressedButton = -1;
					for (int i = 0; i < numCommands; i++) {
						SDL_Rect rect = getButtonBounds(i);
						if (SDL_PointInRect(&point, &rect)) {
							pressedButton = i;
						}
					}
				}

				// Check if track progress bar was clicked...
				{
					SDL_Rect rect = getProgressBarRect();
					if (SDL_PointInRect(&point, &rect)) {
						float newProgress = (point.x - rect.x) * 1.0f / rect.w;
						float newSongTime = newProgress * trackLength;

						check_connection();
						mpd_run_seek_pos(conn, currentSong, newSongTime);
					}
				}
				break;
			}
			case SDL_MOUSEBUTTONUP:
				if (pressedButton > -1 && pressedButton < numCommands) {
					if (int state = check_connection()) {
						quit = true;
						return;
					}

					switch (pressedButton) {
					case BUTTON_PLAYLIST: setDisplayMode(displayMode == CLOCK_MODE ? PLAYLIST_MODE : CLOCK_MODE);
					case CLEANING: if (isCleaningIconVisible()) { lastCleanTime = std::chrono::system_clock::now(); } break;
					case PREV: mpd_run_previous(conn); break;
					case PLAY: mpd_run_play(conn); break;
					case STOP: mpd_run_stop(conn); break;
					case NEXT: mpd_run_next(conn); break;
					case VOL_DOWN: mpd_run_change_volume(conn, -5); break;
					case VOL_UP: mpd_run_change_volume(conn, 5); break;
					case DEATH: {
						mpd_status* status = mpd_run_status(conn);
						int songid = mpd_status_get_song_pos(status);
						mpd_run_delete(conn, songid);
						mpd_status_free(status);
						savePlaylist(this->playlist);
						break;
					}
					case PLAYLIST1: setPlaylist(1); break;
					case PLAYLIST2: setPlaylist(2); break;
					case PLAYLIST3: setPlaylist(3); break;
					case PLAYLIST4: setPlaylist(4); break;
					}

					if (int state = check_connection()) {
						quit = true;
						return;
					}
				}
				pressedButton = -1;
				break;
		}
	}

	void updateShuffle() {
		setShuffleMode(shuffleDefault && displayMode != PLAYLIST_MODE);
	}
	void setDisplayMode(DISPLAY_MODE mode) {
		if (mode != displayMode) {
			displayMode = mode;
			updateShuffle();
		}
	}

	bool isCleaningIconVisible() {
		return false;
//
//		auto now = std::chrono::system_clock::now();
//		auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastCleanTime);
//		float seconds = milliseconds.count() / 1000.0f;
//		float minutes = seconds / 60.0f;
//		float hours = minutes / 60.0f;
//		float days = hours / 24.0f;
//		return days > 30;
	}

	void update() {
		// set night mode
		{
			typedef std::chrono::system_clock Clock;
			auto now = Clock::now();
			std::time_t now_c = Clock::to_time_t(now);
			struct tm *parts = std::localtime(&now_c);
			setNight(parts->tm_hour > 23 || parts->tm_hour < 7);
		}


		// Get Tracklist and track progress
		trackProgress = 0;
		trackLength = 0;
		currentSong = -1;
		check_connection();
		{
			mpd_status* status = mpd_run_status(conn);
			if (status != nullptr) {
				int pos = mpd_status_get_song_pos(status);
				this->currentSong = pos;
				for (int currentPos = 0; currentPos < 5; currentPos++) {
					check_connection();

					int playlistPos = pos - 2 + currentPos;
					mpd_song* song = mpd_run_get_queue_song_pos(conn, playlistPos);
					std::string name = "";
					if (song != nullptr) {
						name = getSongInfo(song, MPD_TAG_ARTIST) + " - " + getSongInfo(song, MPD_TAG_TITLE);
						mpd_song_free(song);
					}
					tracklist[currentPos] = name;
				}

				check_connection();
				if (mpd_status_get_state(status) == MPD_STATE_PLAY ||
					mpd_status_get_state(status) == MPD_STATE_PAUSE) {
					float total = mpd_status_get_total_time(status);
					float elapsed = mpd_status_get_elapsed_time(status);
					if (elapsed > 0) {
						trackProgress = elapsed / total;
					}
					trackLength = total;
				}
				mpd_status_free(status);
			}
		}


	}

	std::string getSongInfo(mpd_song* song, enum mpd_tag_type type) {
		unsigned int i = 0;
		std::string str;
		const char* line;
		while ((line = mpd_song_get_tag(song, type, i++)) != NULL) {
			if (!str.empty()) {
				str = str + " ";
			}
			str += line;
		}
		return str;
	}

	SDL_Rect getProgressBarRect() {
		SDL_Point windowSize;
		SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);

		int h = 40;
		int y = windowSize.y / 2 - 120;
		int marginX = 30;
		int w = windowSize.x - marginX;
		SDL_Rect rect = { marginX/2, y - h/2, w, h } ;
		return rect;
	}

	void render() {
    	SDL_Color c = night ? backgroundNight : backgroundDay;
    	if (isCleaningIconVisible()) {
    		if (night) {
    			c = { 40, 40, 0 };
    		} else {
    			c = { 255, 255, 225 };
    		}
    	}

		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);

        SDL_RenderClear(renderer);

        // render all buttons
        for (int i = 0; i < numCommands; i++) {
        	if (i == CLEANING && !isCleaningIconVisible()) {
        		continue;
        	}

			SDL_Rect rect = getButtonBounds(i);
			int playlist = i - PLAYLIST1 + 1;
			if (i == pressedButton || (playlist >=1 && playlist == this->playlist)) {
				rect.x -= 10;
				rect.y -= 10;
				rect.w += 20;
				rect.h += 20;
			}
			SDL_RenderCopy(renderer, texture[i], 0, &rect);


        }

        // Get window size
		SDL_Point windowSize;
		SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);

        // Render progress bar
        {
        	SDL_Rect rect = getProgressBarRect();
        	if (night) {
        		SDL_SetRenderDrawColor(renderer, 60, 60, 100, 255);
        	} else {
        		SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        	}
			SDL_RenderFillRect(renderer, &rect);
			rect.w = rect.w * getTrackProgress();
			if (night) {
				SDL_SetRenderDrawColor(renderer, 128, 128, 255, 255);
			} else {
				SDL_SetRenderDrawColor(renderer, 200, 200, 255, 255);
			}
			SDL_RenderFillRect(renderer, &rect);
        }

        // render text
		SDL_Color textColor = night ? backgroundDay : backgroundNight;
		SDL_Color disabledTextColor = night ? (SDL_Color) {180, 180, 180, 255} : (SDL_Color) {200, 200, 200, 255} ;

		if (displayMode == CLOCK_MODE) {


			renderText(font30, renderer, getTrackName(), textColor, { windowSize.x / 2, windowSize.y / 2 - 120}, true);
			renderText(font260, renderer, getTime(), textColor, { windowSize.x / 2, windowSize.y / 2 + 40}, true);
			renderText(font30, renderer, getDate(), textColor, { windowSize.x / 2, windowSize.y / 2 + 190}, true);
		} else if (displayMode == PLAYLIST_MODE) {

			int y = 0;
			for (int currentPos = 0; currentPos < 5; currentPos++) {
				SDL_Color lineColor = currentPos == 2 ? textColor : disabledTextColor;
				renderText(font30, renderer, tracklist[currentPos], lineColor, { windowSize.x / 2, windowSize.y / 2 - 50 + y}, true);
				y += 40;
			}

		}

        SDL_RenderPresent(renderer);
	}

	int check_connection() {
		if (mpd_connection_get_error(conn) == MPD_ERROR_SUCCESS) {
			// execute a command to provoke a "connection abort" error, which can only be detected
			// by sending a command to the server and getting a non-success afterwards, from which
			// it cannot be recovered.
			mpd_run_clearerror(conn);
		}

		if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS) {
			fprintf(stderr, "%s\n", mpd_connection_get_error_message(conn));
			if (!mpd_connection_clear_error(conn)) {
				// cannot recover. try to establish a new connection to server
				if (conn != 0) {
					mpd_connection_free(conn);
				}
				fprintf(stderr, "trying to reconnect...\n");
				conn = mpd_connection_new(hostname.c_str(), 0, 30000);
				if (conn == 0) {
					return -1;
				}
				if (mpd_connection_get_error(conn) != MPD_ERROR_SUCCESS)
				{
					fprintf(stderr, "%s\n", mpd_connection_get_error_message(conn));
					return -1;
				}
			}
		}
		return 0;
	}

	SDL_Rect getButtonBounds(int i) {
		int windowWidth, windowHeight;
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);

		if (i == CLEANING) {
			int buttonWidth, buttonHeight;
			SDL_QueryTexture(texture[i], 0, 0, &buttonWidth, &buttonHeight);
			buttonWidth /= 3;
			buttonHeight /= 3;
			int margin = 10;
			SDL_Rect rect = { windowWidth - buttonWidth - margin, windowHeight - buttonHeight - margin, buttonWidth, buttonHeight };
			return rect;
		}

		if (i == BUTTON_PLAYLIST) {
			int buttonWidth, buttonHeight;
			SDL_QueryTexture(texture[i], 0, 0, &buttonWidth, &buttonHeight);
			int margin = 10;
			SDL_Rect rect = { windowWidth - buttonWidth - margin, windowHeight - buttonHeight - margin, buttonWidth, buttonHeight };
			return rect;
		}

		// Toolbar:

		int buttonWidthPlusMargin = windowWidth / LAST_BUTTON_IN_TOOLBAR;
		int buttonWidth, buttonHeight;
		SDL_QueryTexture(texture[i], 0, 0, &buttonWidth, &buttonHeight);
		int margin = buttonWidthPlusMargin - buttonWidth;
		int x = i * buttonWidthPlusMargin;
		SDL_Rect rect = { x + margin / 2, 40, buttonWidth, buttonHeight };
		return rect;
	}

	void setNight(bool night) {
		if (this->night != night) {
			this->night = night;
			nightStatusChanged();
		}
	}

	void nightStatusChanged() {
		if (this->night) {
			dayPlaylist = this->playlist;
			setPlaylist(4);
		} else {
			if (playlist == 4 && dayPlaylist > 0) {
				setPlaylist(dayPlaylist);
			}
		}
	}

	void savePlaylist(int playlist) {
		// before changing
		if (-1 != this->playlist) {
			std::string name = "playlist" + std::to_string(playlist);
			check_connection();
			mpd_run_rm(conn, name.c_str());
			check_connection();
			mpd_run_save(conn, name.c_str());
			check_connection();
		}
	}
	void loadPlaylist(int playlist) {
		// clear current playing buffer (remove all songs)
		mpd_run_clear(conn);
		check_connection();

		// add playlist to buffer
		if (-1 != playlist) {
			std::string name = "playlist" + std::to_string(playlist);
			check_connection();
			mpd_run_load(conn, name.c_str());
			check_connection();
		}
	}
	void setShuffleMode(bool shuffle) {
		mpd_run_random(conn, shuffle);
		check_connection();
	}
	void playRandomSong() {
		// play random song
		unsigned pos = 0;
		if (mpd_status* status = mpd_run_status(conn)) {
			check_connection();
			unsigned length = mpd_status_get_queue_length(status);
			check_connection();
			pos = rand() % length;
			mpd_status_free(status);
		}
		check_connection();

		mpd_run_play_pos(conn, pos);
		check_connection();
	}

	void setPlaylist(int playlist) {
		if (this->playlist != playlist) {
			this->playlist = playlist;

			shuffleDefault = (playlist != 1);
			updateShuffle();

			loadPlaylist(this->playlist);
			playRandomSong();
		}
	}

};

int main(int argc, char ** argv)
{
	std::vector<std::string> args;
	for (int i = 1; i < argc; i++) {
		args.push_back(argv[i]);
	}
	Application app(args);
	return app.run();
}
