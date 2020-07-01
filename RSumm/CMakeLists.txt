PROJECT (Summarizer)

SET (Summ_SRCS 

	src/Main.cpp
	src/Graph.cpp
	src/Sentence.cpp 
	src/Edge.cpp  
	src/CST.cpp 
	src/Salton.cpp 
	src/Util.cpp
	src/Splitter.cpp
	src/Segment.cpp
	src/Topic.cpp
)

SET(Summ_HDRS 
 
	include/Graph.h
	include/Edge.h
	include/Sentence.h
	include/CST.h
	include/Salton.h
	include/Util.h
	include/Splitter.h
	include/Segment.h
	include/Topic.h
	include/Library.h
)

ADD_EXECUTABLE (../bin/RSumm ${Summ_SRCS} ${Summ_HDRS})
