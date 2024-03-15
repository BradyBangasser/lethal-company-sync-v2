package routes

import (
	routes_ping "lsc.bangasser.dev/src/routes/ping"
	routes_game___id__ "lsc.bangasser.dev/src/routes/game/__id__"
	routes_game "lsc.bangasser.dev/src/routes/game"
	routes_report_modpack___pack__ "lsc.bangasser.dev/src/routes/report/modpack/__pack__"
	routes_report_modpack___pack_____mod__ "lsc.bangasser.dev/src/routes/report/modpack/__pack__/__mod__"
	routes_report_modpack "lsc.bangasser.dev/src/routes/report/modpack"
	routes_report "lsc.bangasser.dev/src/routes/report"
	routes_report_mod___id__ "lsc.bangasser.dev/src/routes/report/mod/__id__"
	routes_report_mod "lsc.bangasser.dev/src/routes/report/mod"
	routes_mod "lsc.bangasser.dev/src/routes/mod"
	routes_mod___id___download "lsc.bangasser.dev/src/routes/mod/__id__/download"
	routes_mod___id__ "lsc.bangasser.dev/src/routes/mod/__id__"
	"github.com/gin-gonic/gin"
)

func CreateRouter(r *gin.Engine) {
	r_ping := r.Group("/ping"); {
		r_ping.GET("/", routes_ping.GET)
	}
	r_game := r.Group("/game"); {
		r_game___id__ := r_game.Group("/:id"); {
			r_game___id__.GET("/", routes_game___id__.GET)
		}
		r_game.GET("/", routes_game.GET)
	}
	r_report := r.Group("/report"); {
		r_report_modpack := r_report.Group("/modpack"); {
			r_report_modpack___pack__ := r_report_modpack.Group("/:pack"); {
				r_report_modpack___pack__.POST("/", routes_report_modpack___pack__.POST)
				r_report_modpack___pack_____mod__ := r_report_modpack___pack__.Group("/:mod"); {
					r_report_modpack___pack_____mod__.POST("/", routes_report_modpack___pack_____mod__.POST)
				}
			}
			r_report_modpack.GET("/", routes_report_modpack.GET)
		}
		r_report.GET("/", routes_report.GET)
		r_report_mod := r_report.Group("/mod"); {
			r_report_mod___id__ := r_report_mod.Group("/:id"); {
				r_report_mod___id__.POST("/", routes_report_mod___id__.POST)
			}
			r_report_mod.GET("/", routes_report_mod.GET)
		}
	}
	r.Any("/", ANY)
	r_mod := r.Group("/mod"); {
		r_mod.POST("/", routes_mod.POST)
		r_mod___id__ := r_mod.Group("/:id"); {
			r_mod___id___download := r_mod___id__.Group("/download"); {
				r_mod___id___download.GET("/", routes_mod___id___download.GET)
			}
			r_mod___id__.GET("/", routes_mod___id__.GET)
		}
		r_mod.GET("/", routes_mod.GET)
	}
}
