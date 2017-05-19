#include <mpd/connection.h>
#include <mpd/error.h>
#include <mpd/mixer.h>
#include <mpd/player.h>
#include <mpd/playlist.h>
#include <mpd/queue.h>
#include <mpd/status.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>


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

const int numCommands = 11;


SDL_Color backgroundNight = { 0,0,50,255 };
SDL_Color backgroundDay = {255,255,255,255};


class Application {
private:
	bool night;
	int playlist;

	SDL_Event userEvent;
	int pressedButton = -1;
	SDL_Window * window ;
	SDL_Renderer * renderer;
	SDL_Texture* texture[numCommands];

	const char* imgFiles[numCommands] = {
		"prev.bmp", "play.bmp", "stop.bmp", "next.bmp", "vol_down.bmp", "vol_up.bmp", "playlist1.bmp", "playlist2.bmp", "playlist3.bmp", "playlist4.bmp","death.bmp"};
	enum BUTTONS {
		PREV = 0, PLAY, STOP, NEXT, VOL_DOWN, VOL_UP, PLAYLIST1, PLAYLIST2, PLAYLIST3, PLAYLIST4, DEATH
	};
	struct mpd_connection *conn;

	TTF_Font* font260;
	TTF_Font* font30;

public:

	// init
	Application() {

		userEvent.type = SDL_USEREVENT;
		srand(time(0));
		conn = 0;
		night = false;
		playlist = -1;

		// init SDL
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
		window = SDL_CreateWindow("MPD Client",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1184, 624, 0);

		renderer = SDL_CreateRenderer(window, -1, 0);


		for (int i = 0; i < numCommands; i++) {
			SDL_Surface* surface = SDL_LoadBMP(imgFiles[i]);
			SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
			texture[i] = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_SetTextureBlendMode(texture[i], SDL_BLENDMODE_BLEND);
			SDL_FreeSurface(surface);
		}
		TTF_Init();

		font260 = TTF_OpenFont("Vera.ttf", 260);
		font30 = TTF_OpenFont("Vera.ttf", 30);

		conn = mpd_connection_new("bad", 0, 30000);

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
		SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
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
		stream << std::setw(2) << std::setfill('0') << local->tm_mon;
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
			if (SDL_PollEvent(&event)) {
		        switch (event.type)
		        {
		        	case SDL_QUIT:
		            	quit = true;
		            	break;
		        	case SDL_MOUSEBUTTONDOWN:
		        		pressedButton = -1;
		                for (int i = 0; i < numCommands; i++) {
		        			SDL_Rect rect = getButtonBounds(i);
		        			SDL_Point point = { event.button.x, event.button.y };
		        			if (SDL_PointInRect(&point, &rect)) {
		        				pressedButton = i;
		        			}
		                }
		                break;
					case SDL_MOUSEBUTTONUP:
						if (pressedButton > -1 && pressedButton < numCommands) {
							if (int state = check_connection()) {
								return state;
							}

							switch (pressedButton) {
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
								return state;
							}
						}
						pressedButton = -1;
		    			break;
		        }
		    } else {
		    	SDL_Delay(100);
		    }
	        update();
	        render();
		 }
		 return 0;
	}
	void update() {
		typedef std::chrono::system_clock Clock;
		auto now = Clock::now();
		std::time_t now_c = Clock::to_time_t(now);
		struct tm *parts = std::localtime(&now_c);
		setNight(parts->tm_hour > 20 || parts->tm_hour < 10);

	}

	void render() {

    	SDL_Color c = night ? backgroundNight : backgroundDay;
		SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);

        SDL_RenderClear(renderer);

        // render all buttons
        for (int i = 0; i < numCommands; i++) {
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

        // render text
		SDL_Point windowSize;
		SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);

		SDL_Color textColor = night ? backgroundDay : backgroundNight;
		renderText(font260, renderer, getTime(), textColor, { windowSize.x / 2, windowSize.y / 2}, true);
		renderText(font30, renderer, getDate(), textColor, { windowSize.x / 2, windowSize.y / 2 + 150}, true);

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
				conn = mpd_connection_new("bad", 0, 30000);
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
	    int buttonWidthPlusMargin = windowWidth / numCommands;
		int buttonWidth, buttonHeight;
		SDL_QueryTexture(texture[i], 0, 0, &buttonWidth, &buttonHeight);
		int margin = buttonWidthPlusMargin - buttonWidth;
		int x = i * buttonWidthPlusMargin;
		SDL_Rect rect = {x + margin/2, 40, buttonWidth, buttonHeight };
		return rect;
	}

	void setNight(bool night) {
		if (this->night != night) {
			this->night = night;
			nightStatusChanged();
		}
	}

	void nightStatusChanged() {
//		if (this->night) {
//			setPlaylist(4);
//		} else {
//			if (playlist == 4) {
//				setPlaylist(previousPlaylist);
//			} else if (playlist == -1) {
//				setPlaylist(1);
//			}
//		}
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
			loadPlaylist(this->playlist);
			playRandomSong();
		}
	}

};

int main(int argc, char ** argv)
{
	Application app;
	return app.run();
}
