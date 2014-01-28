IF(NOT DEMI_FIND_FRAMEWORKS_INCLUDED)
  SET(DEMI_FIND_FRAMEWORKS_INCLUDED 1)
  MACRO(DEMI_FIND_FRAMEWORKS fwk)
    IF(APPLE)
      SET(${fwk}_FRAMEWORKS)
      SET(DEMI_FRAMEWORK_PATH
		${DEMI_DEPENDENCIES_DIR}
    	~/Library/Frameworks
    	/Library/Frameworks
    	/System/Library/Frameworks
    	/Network/Library/Frameworks
	  )
	  FOREACH(dir ${DEMI_FRAMEWORK_PATH})
	    SET(fwkpath ${dir}/${fwk}.framework)
	    IF(EXISTS ${fwkpath})
          SET(${fwk}_FRAMEWORKS ${${fwk}_FRAMEWORKS} ${fwkpath})
        ENDIF(EXISTS ${fwkpath})
      ENDFOREACH(dir)
    ENDIF(APPLE)
  ENDMACRO(DEMI_FIND_FRAMEWORKS)
ENDIF(NOT DEMI_FIND_FRAMEWORKS_INCLUDED)
