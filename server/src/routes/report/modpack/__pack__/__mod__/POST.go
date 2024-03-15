package modid

import (
	"fmt"

	"github.com/gin-gonic/gin"
)

//
func POST(c *gin.Context) {
	packId := c.Param("pack")
	modId := c.Param("mod")

	fmt.Printf("I got a report!\n%s, %s\n", packId, modId)

	c.String(200, "That Sucks")
}