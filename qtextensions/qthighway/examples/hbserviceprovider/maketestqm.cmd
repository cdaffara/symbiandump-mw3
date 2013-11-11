copy hbserviceprovider.ts \epoc32\include\platform\qt\translations\hbserviceprovider_en.ts

call lrelease -idbased /epoc32/include/platform/qt/translations/hbserviceprovider_en.ts -qm /epoc32/release/winscw/urel/z/resource/qt/translations/hbserviceprovider_en.qm
call lrelease -idbased /epoc32/include/platform/qt/translations/hbserviceprovider_en.ts -qm /epoc32/release/winscw/udeb/z/resource/qt/translations/hbserviceprovider_en.qm

copy  \epoc32\release\winscw\urel\z\resource\qt\translations\hbserviceprovider_en.qm \epoc32\data\z\resource\qt\translations\*.*
