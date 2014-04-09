#
# Copyright (c) 2013-2014 the Demi3D project.
#

DI_ADD_SAMPLE(Sample_K2World)

# Copy hon resources
if (APPLE AND DEMI_BUILD_PLATFORM_APPLE_IOS)
    add_custom_command(TARGET Sample_K2World POST_BUILD
      COMMAND ditto ${DEMI_SOURCE_DIR}/media_hon ${DEMI_SAMPLE_CONTENTS_PATH}/media_hon
    )
endif()