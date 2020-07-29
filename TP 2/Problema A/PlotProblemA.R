library(comprehenr)
library(ggplot2)

values <- read.table("./5.mse",
                     sep = "\t",
                     header = FALSE)

df <- data.frame(Tipo=rep(c("Entrenamiento", "Validación", "Test"), each=40000/200),
                 epoch=rep(to_vec(for(i in 1:40000) if(i %% 200==0) i), 3),
                 mse=c(values$V5, values$V6, values$V7))

ggplot(data=df, aes(x=epoch, y=mse, group=Tipo)) +
  geom_line(aes(color=Tipo)) +
  geom_point(aes(color=Tipo)) +
  labs(title="Momentum = 0.9, Learning Rate = 0.001\n",x="Épocas de Entrenamiento", y = "Error de Clasificación") +
  theme(plot.title = element_text(hjust = 0.5, face = "bold"))
