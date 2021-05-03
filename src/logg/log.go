package logg

import "log"

// Logger godoc
type Logger struct {
	loglevel int
}

// New godoc
func New(loglevel int) Logger {
	logg := Logger{}
	logg.loglevel = loglevel
	return logg
}

// Print godoc
func (logg Logger) Print(logLevel int, s ...interface{}) {
	if logg.loglevel >= logLevel {
		log.Println(s...)
	}
}
