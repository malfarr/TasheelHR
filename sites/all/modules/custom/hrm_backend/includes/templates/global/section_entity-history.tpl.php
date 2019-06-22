<?php
$entity_class = (isset($variables['entity_class']) ? implode(' ', $variables['entity_class']) : '');
$title = $variables['title'];
$history_logs = $variables['history'];
?>

<div class="entity-history-wrapper <?php echo $entity_class; ?> clearfix">
  <div class="history-log">
      <h3 class="history-log-header"><?php echo $title; ?></h3>                    
      <ul class="history-log-list history-log-hover">
          <?php foreach ($history_logs as $history_log) { ?>
              <li>
                  <?php  echo $history_log['icon']; ?>                
                  <div class="history-log-time"><?php echo $history_log['time']; ?></div>
                  <div class="history-log-content">
                      <p class="push-bit"><strong><?php echo $history_log['title']; ?></strong></p>
                      <p class="push-bit"><?php echo $history_log['content']; ?></p>                    
                  </div>
              </li>        
          <?php } ?>
      </ul>
  </div>
</div>