library(comprehenr)
library(ggplot2)

values <- read.table("./5.mse",
                     sep = "\t",
                     header = FALSE)

df <- data.frame(Tipo=rep(c("Entrenamiento", "Validación", "Test"), each=20000/200),
                 epoch=rep(to_vec(for(i in 1:20000) if(i %% 200==0) i), 3),
                 mse=c(values$V2, values$V3, values$V4))

ggplot(data=df, aes(x=epoch, y=mse, group=Tipo)) +
  geom_line(aes(color=Tipo)) +
  geom_point(aes(color=Tipo)) +
  labs(title="Distribución 50-50",x="Épocas de Entrenamiento", y = "MSE") +
  theme(plot.title = element_text(hjust = 0.5, face = "bold"))
