puntos <- read.table("./Resultados dos_elipses/20.predic",
                     sep = "\t",
                     header = FALSE)

plot(x = 1,
     type = "n",
     xlim = c(-0.6, 0.6), 
     ylim = c(-0.6, 0.6),
     xlab = "", 
     ylab = "",
     main = "Dos Elipses, N_Bins = 20")

segments(-1.5, 0, 1.5, 0)
segments(0, -1.5, 0, 1.5)
     
points(x = puntos$V1[puntos$V3 >= 0.5],
       y = puntos$V2[puntos$V3 >= 0.5],
       pch=16,cex=0.7,
       col = "skyblue3")
points(x = puntos$V1[puntos$V3 < 0.5],
       y = puntos$V2[puntos$V3 < 0.5],
       pch=16,cex=0.7,
       col = "tomato3")

