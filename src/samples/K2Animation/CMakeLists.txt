#
# Copyright (c) 2013-2014 the Demi3D project.
#

DI_ADD_SAMPLE(Sample_K2Animation)

# Copy hon resources
if (APPLE AND DEMI_BUILD_PLATFORM_APPLE_IOS)
#if (FALSE)
    add_custom_command(TARGET Sample_K2Animation POST_BUILD
      COMMAND ditto ${DEMI_SOURCE_DIR}/media_ios/heroes ${DEMI_SAMPLE_CONTENTS_PATH}/media_ios/heroes
    )
endif()